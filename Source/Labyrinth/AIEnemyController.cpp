// Fill out your copyright notice in the Description page of Project Settings.


#include "AIEnemyController.h"

#include "EngineUtils.h"
#include "runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "AIEnemyTargetPoint.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.h"
#include <random>
#include "Kismet/KismetMathLibrary.h"
#include "Runtime/NavigationSystem/Public/NavigationSystem.h"
#include "Runtime/NavigationSystem/Public/NavigationPath.h"
#include "Runtime/Engine/Public/DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "LabyrinthGameStateBase.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerState.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LabBlock.h"
#include <algorithm>
#include "PuzzleActor.h"
#include "PuzzleTools.h"
#include "SolvableActor.h"
#include "GameFramework/CharacterMovementComponent.h"


AAIEnemyController::AAIEnemyController() {
	// Setup the perception component
	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));
	UAISenseConfig_Sight* sightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("Sight Config"));
	sightConfig->SightRadius = SightRadius;
	sightConfig->LoseSightRadius = 1.1 * SightRadius;
	sightConfig->PeripheralVisionAngleDegrees = 90.0f;
	sightConfig->DetectionByAffiliation.bDetectEnemies = true;
	sightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	sightConfig->DetectionByAffiliation.bDetectNeutrals = true;
	PerceptionComponent->ConfigureSense(*sightConfig);
	PerceptionComponent->SetDominantSense(sightConfig->GetSenseImplementation());
	PerceptionComponent->OnPerceptionUpdated.AddDynamic(this, &AAIEnemyController::Sensing);
}

void AAIEnemyController::BeginPlay() {
	Super::BeginPlay();
}

/** Sera utilisé par la tâche UpdateNextTargetPointBTTaskNode du
 Behavior Tree pour actualiser le chemin de patrouille */
void AAIEnemyController::UpdateNextTargetPoint()
{

	APawn* PawnUsed = GetPawn();

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	AAIEnemyTargetPoint* TargetPoint = Cast<AAIEnemyTargetPoint>(BlackboardComponent->GetValueAsObject("TargetPoint"));

	if (TargetPoint == nullptr || FVector::Dist(PawnUsed->GetActorLocation(), TargetPoint->GetActorLocation()) < 300.0f)
	{
		TArray<AActor*> tps;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIEnemyTargetPoint::StaticClass(), tps);
		
		TArray<AActor*> partition = tps.FilterByPredicate([&](AActor* tp) {
			if (TargetPoint != nullptr) {
				return FVector::Dist(PawnUsed->GetActorLocation(), tp->GetActorLocation()) < 5 * LabBlock::assetSize && Cast<AAIEnemyTargetPoint>(tp)->Position != TargetPoint->Position;
			}
			return FVector::Dist(PawnUsed->GetActorLocation(), tp->GetActorLocation()) < 5 * LabBlock::assetSize;
		});

		if (partition.Num() <= 1) {
			BlackboardComponent->SetValueAsObject("TargetPoint", PreviousTargetPoint);
		}
		else {

			std::random_device rd;
			std::mt19937 prng{ rd() };
			std::uniform_int_distribution<int> tp_Rd{ 0, partition.Num() - 1 };

			AAIEnemyTargetPoint* newTP;
			do {
				newTP = Cast<AAIEnemyTargetPoint>(partition[tp_Rd(prng)]);
			} while (newTP == PreviousTargetPoint);

			PreviousTargetPoint = TargetPoint;
			BlackboardComponent->SetValueAsObject("TargetPoint", newTP);
		}
	}
}

void AAIEnemyController::Sensing(const TArray<AActor*>& actors) {
	for (AActor* actor : actors) {

		FActorPerceptionBlueprintInfo info;
		PerceptionComponent->GetActorsPerception(actor, info);

		UBlackboardComponent* blackboard = GetBrainComponent()->GetBlackboardComponent();

		APlayerCharacter* player = Cast<APlayerCharacter>(actor);
		// actor is a player
		if (player) {

			AActor* PlayerActor = Cast<AActor>(blackboard->GetValueAsObject("TargetActorToFollow"));

			// IN SIGHT
			if (info.LastSensedStimuli[0].WasSuccessfullySensed()) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Now I see you!");

				AActor* currentTarget = Cast<AActor>(blackboard->GetValueAsObject("TargetActorToFollow"));

				FVector newSeenPos = actor->GetActorLocation();
				UNavigationPath* path2 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), newSeenPos, this);

				if (currentTarget != nullptr) {
					FVector currentTargetPos = currentTarget->GetActorLocation();
					UNavigationPath* path1 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), currentTargetPos, this);

					if (path1->IsValid() && !path1->IsPartial() && path2->IsValid() && !path2->IsPartial()) {
						if (path1->GetPathLength() > path2->GetPathLength())
							blackboard->SetValueAsObject("TargetActorToFollow", actor);
					}
				}
				else {
					if (path2->IsValid() && !path2->IsPartial())
						blackboard->SetValueAsObject("TargetActorToFollow", actor);
				}
			}
			// SIGHT LOST
			else if (actor == PlayerActor) {
				GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Now I don't !");
				blackboard->ClearValue("TargetActorToFollow");
				float dist_min = INFINITY;
				AAIEnemyTargetPoint* point1 = nullptr;
				AAIEnemyTargetPoint* point2 = nullptr;

				float pathLength1;
				float pathLength2;

				for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It) {

					UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), It->GetActorLocation(), PlayerActor);

					if (!path->IsPartial()) {
						if (path->GetPathLength() < dist_min) {
							dist_min = path->GetPathLength();
							point1 = *It;
						}
					}

				}

				if (point1 != nullptr) {
					pathLength1 = dist_min;
					dist_min = INFINITY;

					for (TActorIterator<AAIEnemyTargetPoint> It(GetWorld()); It; ++It) {

						if (point1->Position != It->Position) {
							UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), It->GetActorLocation(), PlayerActor);

							if (!path->IsPartial()) {
								if (path->GetPathLength() < dist_min) {
									dist_min = path->GetPathLength();
									point2 = *It;
								}
							}
						}
					}
					pathLength2 = dist_min;

					// DEBUG LINE
					DrawDebugLine(GetWorld(), point1->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, point2->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, FColor{ 255,0,0 }, false, 5.f, (uint8)'\000', 10.f);

					if (pathLength1 < pathLength2) {
						blackboard->SetValueAsObject("TargetPoint", point2);
					}
					else {
						blackboard->SetValueAsObject("TargetPoint", point1);
					}
				}

			}

		}
		else if (!blackboard->GetValueAsObject("TargetActorToFollow")) {
			if (info.LastSensedStimuli[0].WasSuccessfullySensed()) {
				CheckElementChangedState(actor);
			}
		}

	}
	
}


//void AAIEnemyController::SenseSight()
//{
//
//	APawn* PawnUsed = GetPawn();
//
//	//
//	//AGameStateBase* GameState = GetWorld()->GetGameState<ALabyrinthGameStateBase>();
//	//TArray<APlayerState*> PlayerArray = GameState->PlayerArray;
//
//	//TArray<APlayerState*> ArrayFiltered = PlayerArray.FilterByPredicate([&](APlayerState* Player) {
//
//	//	if (Player->IsABot())
//	//		return false;
//
//	//	APawn* PlayerPawn = Player->GetPawn();
//	//	FVector PositionAI = PawnUsed->GetActorLocation();
//	//	FVector PositionPlayer = PlayerPawn->GetActorLocation();
//
//	//	FVector DistanceBetween = PositionPlayer - PositionAI;
//	//	DistanceBetween.FVector::Normalize();
//
//	//	FVector FVectorAI = PawnUsed->GetActorForwardVector();
//
//	//	float DotProd = FVector::DotProduct(DistanceBetween, FVectorAI);
//
//	//	if (UKismetMathLibrary::DegAcos(DotProd) < 90.0f)
//	//	{
//	//		AActor* AIActor = GetOwner();
//	//		UCapsuleComponent* capsule = Cast<ACharacter>(PawnUsed)->GetCapsuleComponent();
//	//		FCollisionQueryParams TraceParams(FName(TEXT("TraceAI2Player")), false, capsule->GetOwner());
//	//		//TraceParams.bReturnPhysicalMaterial = false;
//	//		//TraceParams.bTraceComplex = false;
//
//	//		FHitResult Hit(ForceInit);
//	//		GetWorld()->LineTraceSingleByChannel(Hit, PositionAI, PositionPlayer, ECC_Camera, TraceParams);
//	//		APlayerCharacter* CastedActor = Cast<APlayerCharacter>(Hit.GetActor());
//
//	//		if (!CastedActor) {
//	//			return false;
//	//		}
//	//		else {
//	//			return true;
//	//		}
//	//	}
//
//	//	else
//	//		return false;
//
//	//	});
//
//	//ArrayFiltered.Sort([&](const APlayerState& PlayerState1, const APlayerState& PlayerState2) {
//
//	//	APawn* PlayerPawn1 = PlayerState1.GetPawn();
//	//	FVector PositionAI = PawnUsed->GetActorLocation();
//	//	FVector PositionPlayer1 = PlayerPawn1->GetActorLocation();
//
//	//	APawn* PlayerPawn2 = PlayerState2.GetPawn();
//	//	FVector PositionPlayer2 = PlayerPawn2->GetActorLocation();
//
//	//	float distance1 = FVector::Distance(PositionAI, PositionPlayer1);
//
//	//	float distance2 = FVector::Distance(PositionAI, PositionPlayer2);
//
//	//	return distance1 < distance2;
//
//	//	});
//
//	//return ArrayFiltered;
//	
//
//
//	/*UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
//
//	if (PlayerCharacters.Num() == 0 && PlayerToFollow == nullptr) {
//		BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
//	}
//	else if (PlayerCharacters.Num() == 0 && PlayerToFollow != nullptr) {
//		auto iterable = PlayersLastSeen.find(PlayerToFollow->GetUniqueID());
//		if (iterable != PlayersLastSeen.end() && iterable->second >= 5.0f) {
//			BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
//		}
//		else {
//			BlackboardComponent->SetValueAsObject("TargetActorToFollow", PlayerToFollow);
//		}
//	}
//	else if (PlayerCharacters.Num() != 0) {
//
//		//ACharacter* PlayerCharacter = Cast<ACharacter>((*PlayerCharacters.begin())->GetPawn());
//		//APlayerCharacter* PlayerPlayerCharacter = Cast<APlayerCharacter>(PlayerCharacter);
//
//		int i = 0;
//		for (auto it : PlayerCharacters) {
//			++i;
//			APlayerCharacter* PlayerCharacter = Cast<APlayerCharacter>(it->GetPawn());
//			if (PlayerCharacter->IsHidden() && i == PlayerCharacters.Num()) {
//				BlackboardComponent->SetValueAsObject("TargetActorToFollow", NULL);
//			}
//			else if (!PlayerCharacter->IsHidden()) {
//				BlackboardComponent->SetValueAsObject("TargetActorToFollow", PlayerCharacter);
//				PlayerToFollow = PlayerCharacter;
//				break;
//			}
//		}
//	}*/
//
//}

//void AAIEnemyController::SenseHearing() {
//
//}

EPathFollowingRequestResult::Type AAIEnemyController::MoveToEnemy()
{

	//UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	//// Obtenir un pointeur sur le personnage référencé par la clé TargetActorToFollow du BlackBoard
	//AActor* PlayerActor = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActorToFollow"));

	//UCapsuleComponent* capsule = Cast<ACharacter>(GetPawn())->GetCapsuleComponent();
	//FCollisionQueryParams TraceParams(FName(TEXT("TraceAI2Player")), false, capsule->GetOwner());
	//
	//FHitResult Hit(ForceInit);

	//FVector PositionAI = GetPawn()->GetActorLocation();
	//FVector PositionPlayer = PlayerActor->GetActorLocation();
	//GetWorld()->LineTraceSingleByChannel(Hit, PositionAI, PositionPlayer, ECC_Camera, TraceParams);

	//APlayerCharacter* HitActor = Cast<APlayerCharacter>(Hit.GetActor());

	//if (HitActor != nullptr) {
	//	MoveToActor(HitActor);
	//}
	//// SIGHT LOST
	//else {
	//	
	//}	
	return EPathFollowingRequestResult::RequestSuccessful;
}

void AAIEnemyController::CheckElementChangedState(AActor* actor)
{
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();

	if (!BlackboardComponent->GetValueAsObject("PuzzleToInvestigate")) {

		APuzzleActor* puzzle = Cast<APuzzleActor>(actor);
		if (puzzle) {
			if (puzzle->GetEtat() == -1) {
				BlackboardComponent->SetValueAsObject("PuzzleToInvestigate", actor);
				PuzzlesInMemory.Add(puzzle, puzzle->GetEtat());

				FVector forward = actor->GetActorForwardVector();
				forward.Normalize();
				FVector position = actor->GetActorLocation() + forward * 200.f;
				BlackboardComponent->SetValueAsVector("PuzzlePosition", position);
			}
			else if (PuzzlesInMemory.Contains(puzzle)) {
				if (PuzzlesInMemory[puzzle] != puzzle->GetEtat()) {
					BlackboardComponent->SetValueAsObject("PuzzleToInvestigate", actor);

					FVector forward = actor->GetActorForwardVector();
					forward.Normalize();
					FVector position = actor->GetActorLocation() + forward * 200.f;
					BlackboardComponent->SetValueAsVector("PuzzlePosition", position);
				}
				PuzzlesInMemory[puzzle] = puzzle->GetEtat();
			}
			else if (!PuzzlesInMemory.Contains(puzzle)) {
				PuzzlesInMemory.Add(puzzle, puzzle->GetEtat());
			}
		}
	}
}

void AAIEnemyController::CheckPuzzlesToInvestigate()
{
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	AActor* actorInvestigate = Cast<AActor>(BlackboardComponent->GetValueAsObject("PuzzleToInvestigate"));

	/*FVector positionAI = GetPawn()->GetActorLocation();
	FVector positionPuzzle = actorInvestigate->GetActorLocation();
	FVector directionAIPuzzle = positionAI - positionPuzzle;
	directionAIPuzzle.Normalize();

	float DotProd = FVector::DotProduct(directionAIPuzzle, GetPawn()->GetActorForwardVector());
	float angle = UKismetMathLibrary::DegAcos(DotProd);*/

	APuzzleActor* puzzleInvestigate = Cast<APuzzleActor>(actorInvestigate);
	if (puzzleInvestigate) {
		if (puzzleInvestigate->GetEtat() != -1) {
			BlackboardComponent->ClearValue("PuzzleToInvestigate");
			BlackboardComponent->SetValueAsVector("PlaceToInvestigate", puzzleInvestigate->GetActorLocation());
			return;
		}

		TArray<FLinkedActors> solvables = puzzleInvestigate->targetActor;
		
		float dist_min = INFINITY;
		ASolvableActor* solvable = nullptr;
		for (FLinkedActors linked : solvables) {
			if (linked.linkedActor != nullptr) {
				UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetPawn()->GetActorLocation(), linked.linkedActor);

				if (!path->IsPartial()) {
					if (path->GetPathLength() < dist_min) {
						dist_min = path->GetPathLength();
						solvable = linked.linkedActor;
					}
				}
			}
		}

		if (solvable != nullptr) {
			BlackboardComponent->SetValueAsObject("PuzzleToInvestigate", solvable);
			FVector forward = solvable->GetActorForwardVector();
			forward.Normalize();
			BlackboardComponent->SetValueAsVector("PuzzlePosition", solvable->GetActorLocation() + forward * 200.f);
		}
		else
			BlackboardComponent->ClearValue("PuzzleToInvestigate");
	}
}

void AAIEnemyController::UpdateFocus()
{
	SetFocus(nullptr);

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	AActor* actorInvestigate = Cast<AActor>(BlackboardComponent->GetValueAsObject("PuzzleToInvestigate"));

	if (actorInvestigate && FVector::Distance(actorInvestigate->GetActorLocation(), GetPawn()->GetActorLocation()) < 750.0f) {
		SetFocus(actorInvestigate);
		//GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 100;
	}
}

void AAIEnemyController::Wander() {

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	FVector placeToInvestigate = BlackboardComponent->GetValueAsVector("PlaceToInvestigate");

	FVector location = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), placeToInvestigate, 2 * LabBlock::assetSize);
	BlackboardComponent->SetValueAsVector("WanderPoint", location);
}