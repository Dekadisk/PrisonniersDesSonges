#include "AIDirector.h"
#include "PlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIEnemyController.h"
#include "SolvableActor.h"
#include "ResponseCurve.h"
#include "AIEnemyTargetPoint.h"
#include "LabyrinthGameModeBase.h"
#include "LabGenerator.h"
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
	if (MonstersArray.Num() > 0)
		Monster = Cast<AAIEnemyController>(MonstersArray[0]);

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASolvableActor::StaticClass(), Solvables);

}

// Called every frame
void AAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UBrainComponent* brain = Monster->GetBrainComponent();
	UBlackboardComponent* blackboard = brain->GetBlackboardComponent();

	if (blackboard->IsVectorValueSet("PlaceToInvestigate"))
		timeWandering += DeltaTime;
	else
		timeWandering = 0.0f;
	
	if (!blackboard->GetValueAsObject("TargetActorToFollow") && !blackboard->GetValueAsObject("PuzzleToInvestigate") && !blackboard->GetValueAsObject("PriorityTargetPoint") && !blackboard->IsVectorValueSet("PlaceToInvestigate"))
		timePatrolling += DeltaTime;
	else
		timePatrolling = 0.0f;

	VerifyPlayersAlive();

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
		DebugDisplayInfo();
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
		UInfluenceDataAsset* data = Cast<APlayerCharacter>(Cast<APlayerController>(pair.Key)->GetPawn())->InfluenceDataAsset;
		data->alternativeInfluence = data->influence / (1 + pair.Value);
	}
}

AActor* AAIDirector::NextTarget()
{
	// A MODIFIER : PRENDRE EN COMPTE L'INFLUENCE MAP <-------------------------------------------------------
	float maxPriority = 0.0f;
	AActor* nextTarget = nullptr;
	UNavigationPath* path;

	TArray<AActor*> tps;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AAIEnemyTargetPoint::StaticClass(), FName(FString::FromInt(Monster->currentSection)), tps);
	ALabGenerator* labGen = Cast<ALabGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ALabGenerator::StaticClass()));	
	
	for (AActor* tp : tps) {
		float inf = labGen->GetCellInfluenceAtPos(tp->GetActorLocation());
		if (inf > maxPriority) {
			path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), tp->GetActorLocation(), Monster);
			if (path->IsValid() && !path->IsPartial()) {
				maxPriority = inf;
				nextTarget = tp;
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
	if (!path->IsValid())
		return -1.0f;

	float dist = path->GetPathLength();
	if (dist > Monster->DataAsset->ThreateningDist)
		return -1.0f;

	dist = dist / Monster->DataAsset->ThreateningDist;
	float res = ResponseCurve::Calculate(c_type::Logistic, dist, 10.0f, 1.0f, -0.3f, -1.0f);
	return res;
	
}

void AAIDirector::VerifyPlayersAlive()
{
	TArray<AActor*> ToRemove;
	for (AActor* player : Players) {
		if (!Cast<APlayerCharacter>(Cast<APlayerController>(player)->GetPawn()))
			ToRemove.Add(player);
	}

	for (AActor* removal : ToRemove) {
		Players.Remove(removal);
		Threats.Remove(removal);
	}
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
		AActor* priority = NextTarget();
		blackboard->SetValueAsObject("PriorityTargetPoint", priority);
		/*if (player != nullptr) {
			const FVector playerPos = Cast<APlayerController>(player)->GetPawn()->GetActorLocation();
			TArray<AActor*> tps;
			UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AAIEnemyTargetPoint::StaticClass(), FName(FString::FromInt(Monster->currentSection)), tps);

			AActor* best;
			UNavigationPath* path;
			do {
				float min_dist = INFINITY;
				best = nullptr;
				for (AActor* tp : tps) {
					float dist = FVector::Dist(tp->GetActorLocation(), playerPos);
					if (dist < min_dist) {
						min_dist = dist;
						best = tp;
					}						
				}
				tps.Remove(best);
				path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), best->GetActorLocation(), Monster);
			} while (!path->IsValid() || path->IsPartial());

			blackboard->SetValueAsObject("PriorityTargetPoint", best);
		}*/		
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

		GEngine->AddOnScreenDebugMessage(i, 1.1f, FColor::Red, FString("WOW, tellement de joueurs : ") + FString::FromInt(Players.Num()));
	}
}