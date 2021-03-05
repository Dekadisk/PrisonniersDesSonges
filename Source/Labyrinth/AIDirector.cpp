// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDirector.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "AIEnemyController.h"
#include "SolvableActor.h"
#include "ResponseCurve.h"
#include "AIEnemyTargetPoint.h"
#include "LabyrinthGameModeBase.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include <Runtime/AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Object.h>
#include <Runtime/AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Bool.h>
#include <Runtime/AIModule/Classes/BehaviorTree/Blackboard/BlackboardKeyType_Vector.h>
#include "Algo/Find.h"

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

	UpdateThreats(DeltaTime);
	
	DirectMonster();

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
		if (GEngine) {
			GEngine->AddOnScreenDebugMessage(4, 1.1f, FColor::Yellow, FString::SanitizeFloat(toAdd));
		}
		pair.Value += 0.1 * toAdd;
		pair.Value = FMath::Clamp(pair.Value, 0.0f, 1.0f);
	}
}

AActor* AAIDirector::NextPlayerTarget()
{
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
	// Logistic par rapport à distance
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

	//blackboard->SetValueAsBool(FName("QuentinEstLePlusBeau"), true);

	// Give a zone to go when monster is lost
	if (!blackboard->GetValueAsBool("Investigating") && !blackboard->GetValueAsObject("TargetActorToFollow")->IsValidLowLevel())
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

			blackboard->SetValueAsObject("TargetPoint", target);
		}		
	}
}

void AAIDirector::DebugDisplayInfo() {
	int i = 1;
	if (GEngine) {
		//GEngine->AddOnScreenDebugMessage(0, 1.1f, FColor::Blue, FString::Printf(TEXT("Nous avons %d enigmes. Ca va chauffer le cerveau L O L !"), Solvables.Num()));
		//GEngine->AddOnScreenDebugMessage(1, 1.1f, FColor::Blue, FString::Printf(TEXT("Nous avons %d joueurs. C'est incroyable tout ca lo !"), Players.Num()));
		GEngine->AddOnScreenDebugMessage(0, 1.1f, FColor::Green, TEXT("Checkez moi ca les menaces :"));
		for (const TPair<AActor*, float>& pair : Threats)
		{
			FString name = UKismetSystemLibrary::GetObjectName(pair.Key);
			name += ", Un des boss du jeu : " + FString::SanitizeFloat(pair.Value);
			GEngine->AddOnScreenDebugMessage(i, 1.1f, FColor::Red, name);
			i++;
		}

		UBrainComponent* brain = Monster->GetBrainComponent();
		UBlackboardComponent* blackboard = brain->GetBlackboardComponent();

		GEngine->AddOnScreenDebugMessage(i+1, 1.1f, FColor::Green, TEXT("Les valeurs du BBC :"));
		for (int j = 0; j < blackboard->GetNumKeys(); j++) {
			FName key = blackboard->GetKeyName(j);
			TSubclassOf<UBlackboardKeyType> type = blackboard->GetKeyType(j);
			FString text;
			if (type->GetName() == UBlackboardKeyType_Object::StaticClass()->GetName()) {
				UObject* obj = blackboard->GetValueAsObject(key);
				FString s = "Invalide";
				if(obj->IsValidLowLevel())
					s = obj->GetName();
				text += key.ToString() + " : " + s;
				GEngine->AddOnScreenDebugMessage(i + j, 1.1f, FColor::Blue, text);
			}
			else if (type->GetName() == UBlackboardKeyType_Bool::StaticClass()->GetName()) {
				FString s = blackboard->GetValueAsBool(key) ? "True" : "False";
				text += key.ToString() + " : " + s;
				GEngine->AddOnScreenDebugMessage(i + j, 1.1f, FColor::Magenta, text);
			}
			else if (type->GetName() == UBlackboardKeyType_Vector::StaticClass()->GetName()) {
				FString s = blackboard->GetValueAsVector(key).ToCompactString();
				text += key.ToString() + " : " + s;
				GEngine->AddOnScreenDebugMessage(i + j, 1.1f, FColor::Yellow, text);
			}
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
