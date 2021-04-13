#include "AIDirector.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIEnemyController.h"
#include "SolvableActor.h"
#include "ResponseCurve.h"
#include "AIEnemyTargetPoint.h"
#include "LabyrinthGameModeBase.h"
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Object.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Bool.h"
#include "BehaviorTree/Blackboard/BlackboardKeyType_Vector.h"

// Sets default values
AAIDirector::AAIDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bNetLoadOnClient = false;

	SetActorTickInterval(tickRate);
}

// Called when the game starts or when spawned
void AAIDirector::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> MonstersArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIEnemyController::StaticClass(), MonstersArray);
	if (MonstersArray.Num() > 0) {
		Monster = Cast<AAIEnemyController>(MonstersArray[0]);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASolvableActor::StaticClass(), Solvables);

}

// Called every frame
void AAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UBrainComponent* brain = Monster->GetBrainComponent();
	UBlackboardComponent* blackboard = brain->GetBlackboardComponent();

	if (blackboard->IsVectorValueSet("PlaceToInvestigate")) {
		timeWandering += DeltaTime;
	}
	else {
		timeWandering = 0.0f;
	}
	
	if (!blackboard->GetValueAsObject("TargetActorToFollow") && !blackboard->GetValueAsObject("PuzzleToInvestigate") && !blackboard->GetValueAsObject("PriorityTargetPoint") && !blackboard->IsVectorValueSet("PlaceToInvestigate")) {
		timePatrolling += DeltaTime;
	}
	else {
		timePatrolling = 0.0f;
	}

	UpdateThreats(DeltaTime);

	if (timeWandering >= stopWandering || timePatrolling >= stopPatrolling || blackboard->GetValueAsObject("PuzzleToInvestigate")) {
		DirectMonster();
		timeWandering = 0.0f;
		timePatrolling = 0.0f;
		blackboard->ClearValue("PlaceToInvestigate");
		blackboard->ClearValue("WanderPoint");
		blackboard->ClearValue("TargetPoint");
	}

	if (Cast<ALabyrinthGameModeBase>(GetWorld()->GetAuthGameMode())->debug)
	{
		DebugDisplayInfo();
	}
}

void AAIDirector::AddPlayer(AActor* actor) {
	Players.Add(actor);
	Threats.Add(actor, 0.f);
}

void AAIDirector::UpdateThreats(float DeltaTime)
{
	for (TPair<AActor*, float>& pair : Threats)
	{
		float toAdd = GenerateThreat(pair.Key);
		pair.Value += 0.1 * toAdd;
		pair.Value = FMath::Clamp(pair.Value, 0.0f, 1.0f);
	}
}

AActor* AAIDirector::NextPlayerTarget()
{
	// A MODIFIER : PRENDRE EN COMPTE L'INFLUENCE MAP <-------------------------------------------------------
	float maxPriority = 0.0f;
	AActor* nextTarget = nullptr;
	float meanDist = CalculateMeanDistToPlayers();
	for (AActor* player : Players) {
		FVector playerPos = Cast<APlayerController>(player)->GetPawn()->GetActorLocation();
		UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), playerPos, Monster);
		if (path->IsValid() && !path->IsPartial())
		{
			float priority = (1.0f - Threats[player]);
			if (priority > maxPriority) {
				maxPriority = priority;
				nextTarget = player;
			}
		}		
	}

	return nextTarget;
}

float AAIDirector::GenerateThreat(AActor* player)
{
	using c_type = ResponseCurve::CurveType;
	// Logistic par rapport � distance
	// Atteindre le max en un certain temps
	FVector playerPos = Cast<APlayerController>(player)->GetPawn()->GetActorLocation();
	UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), playerPos, Monster);
	if (!path->IsValid()) {
		return -1.0f;
	}
	float dist = path->GetPathLength();
	if (dist > Monster->ThreateningDist)
		return -1.0f;
	dist = dist / Monster->ThreateningDist;
	float res = ResponseCurve::Calculate(c_type::Logistic, dist, 10.0f, 1.0f, -0.3f, -1.0f);
	return res;
}

void AAIDirector::DirectMonster()
{
	UBrainComponent* brain = Monster->GetBrainComponent();
	UBlackboardComponent* blackboard = brain->GetBlackboardComponent();

	AActor* puzzle = Cast<AActor>(blackboard->GetValueAsObject("PuzzleToInvestigate"));
	ASolvableActor* solvable = Cast<ASolvableActor>(puzzle);
	if (solvable && !solvable->isSolved) {

		// A MODIFIER : PRENDRE EN COMPTE L'INFLUENCE MAP <-------------------------------------------------------
		AUsableActor** puzzleToBoloss = solvable->Elements.FindByPredicate([&](AUsableActor* puzzle) {
			return puzzle->GetEtat() == -1;
		});
		if(puzzleToBoloss)
			blackboard->SetValueAsObject("PuzzleToBoloss", *puzzleToBoloss);
	}

	// Give a zone to go when monster is lost
	if (!puzzle && !blackboard->GetValueAsObject("TargetActorToFollow")->IsValidLowLevel())
	{
		const AActor* player = NextPlayerTarget();
		if (player != nullptr) {
			const FVector playerPos = Cast<APlayerController>(player)->GetPawn()->GetActorLocation();
			TArray<AActor*> tps;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIEnemyTargetPoint::StaticClass(), tps);

			tps.Sort([&](const AActor& tp1, const AActor& tp2) {
				return FVector::Dist(tp1.GetActorLocation(), playerPos) < FVector::Dist(tp2.GetActorLocation(), playerPos);
				});

			AActor* target = *tps.FindByPredicate([&](AActor* tp) {
				UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), tp->GetActorLocation(), Monster);
				return path->IsValid() && !path->IsPartial();
				});

			blackboard->SetValueAsObject("PriorityTargetPoint", target);
		}		
	}
}

float AAIDirector::CalculateMeanDistToPlayers()
{
	float mean = 0.0f;
	int div = 0;
	for (AActor* player : Players) {
		FVector playerPos = Cast<APlayerController>(player)->GetPawn()->GetActorLocation();
		UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), playerPos, Monster);
		if (path->IsValid() && !path->IsPartial())
		{
			mean += path->GetPathLength();
			div++;
		}
	}
	if (div == 0)
		return 1.0f;
	mean /= div;
	return mean;
}

void AAIDirector::DebugDisplayInfo() {
	int i = 4;
	if (GEngine) {
		
		GEngine->AddOnScreenDebugMessage(0, 1.1f, FColor::Green, TEXT("Le temps c'est de l'argent :"));
		GEngine->AddOnScreenDebugMessage(1, 1.1f, FColor::Green, FString("timeWandering = ") + FString::SanitizeFloat(timeWandering));
		GEngine->AddOnScreenDebugMessage(2, 1.1f, FColor::Green, FString("timePatrolling = ") + FString::SanitizeFloat(timePatrolling));

		GEngine->AddOnScreenDebugMessage(3, 1.1f, FColor::Green, TEXT("Checkez moi ca les menaces :"));
		for (const TPair<AActor*, float>& pair : Threats)
		{
			FString name = UKismetSystemLibrary::GetObjectName(pair.Key);
			name += ", Un des boss du jeu : " + FString::SanitizeFloat(pair.Value);
			GEngine->AddOnScreenDebugMessage(i, 1.1f, FColor::Red, name);
			i++;
		}
	}
}