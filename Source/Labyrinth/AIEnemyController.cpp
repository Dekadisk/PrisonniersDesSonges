#include "AIEnemyController.h"
#include "EngineUtils.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "AIEnemyTargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include <random>
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LabBlock.h"
#include "PuzzleActor.h"
#include "PuzzleTools.h"
#include "SolvableActor.h"

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

/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du
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
				return Cast<AAIEnemyTargetPoint>(tp)->Position != TargetPoint->Position && FString::FromInt(currentSection) == tp->Tags[0].ToString();
			}
			return FString::FromInt(currentSection) == tp->Tags[0].ToString();
		});

		partition.Sort([&](AActor& tp1, AActor& tp2) {
			return FVector::Dist(PawnUsed->GetActorLocation(), tp1.GetActorLocation()) < FVector::Dist(PawnUsed->GetActorLocation(), tp2.GetActorLocation());
		});

		if (partition.Num() == 0) {
			BlackboardComponent->SetValueAsObject("TargetPoint", PreviousTargetPoint);
		}
		else {
			TArray<AActor*> tpsProches;
			for (int i = 0; i < std::min(partition.Num(), 4); ++i)
				tpsProches.Add(partition[i]);

			TArray<AActor*> tpsTresProches;
			for (AActor* tp : tpsProches) {
				auto path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), PawnUsed->GetActorLocation(), tp);
				if (path->GetPathLength() < 5 * LabBlock::assetSize)
					tpsTresProches.Add(tp);
			}

			std::random_device rd;
			std::mt19937 prng{ rd() };
			std::uniform_int_distribution<int> tp_Rd{ 0, tpsTresProches.Num() - 1 };

			AAIEnemyTargetPoint* newTP;
			do {
				newTP = Cast<AAIEnemyTargetPoint>(tpsTresProches[tp_Rd(prng)]);
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

						if (point1->GetName() != It->GetName()) {
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
						blackboard->SetValueAsObject("PriorityTargetPoint", point2);
					}
					else {
						blackboard->SetValueAsObject("PriorityTargetPoint", point1);
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
//}

//void AAIEnemyController::SenseHearing() {
//
//}

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
			FVector pos = solvable->GetActorLocation() + 200.0f * forward;
			FCollisionQueryParams TraceParams(FName(TEXT("ProjectOnGround")), false, solvable);
			TraceParams.bReturnPhysicalMaterial = false;
			TraceParams.bTraceComplex = false;
			
			FHitResult Hit(ForceInit);
			GetWorld()->LineTraceSingleByChannel(Hit, pos, pos - FVector{ 0,0,1000 }, ECC_Camera, TraceParams);
			FVector pause = UNavigationSystemV1::ProjectPointToNavigation(GetWorld(), Hit.Location);
			BlackboardComponent->SetValueAsVector("PuzzlePosition", pause);
		}
		else
			BlackboardComponent->ClearValue("PuzzleToInvestigate");
	}
	else {
		ASolvableActor* solvable = Cast<ASolvableActor>(actorInvestigate);
		if (solvable && solvable->isSolved) {
			currentSection++;
			TArray<AActor*> tps;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIEnemyTargetPoint::StaticClass(), tps);
			TArray<AActor*> sectionTPs = tps.FilterByPredicate([&](AActor* tp) {
				return tp->Tags[0].ToString() == FString::FromInt(currentSection);
			});
			sectionTPs.Sort([&](AActor& tp1, AActor& tp2) {
				return FVector::Dist(GetPawn()->GetActorLocation(), tp1.GetActorLocation()) < FVector::Dist(GetPawn()->GetActorLocation(), tp2.GetActorLocation());
			});

			BlackboardComponent->SetValueAsObject("NewZoneTargetPoint", sectionTPs[0]);
			BlackboardComponent->ClearValue("PuzzleToInvestigate");
		}
	}
}

void AAIEnemyController::UpdateFocus()
{
	SetFocus(nullptr);

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	AActor* actorInvestigate = Cast<AActor>(BlackboardComponent->GetValueAsObject("PuzzleToInvestigate"));
	AActor* actorBoloss = Cast<AActor>(BlackboardComponent->GetValueAsObject("PuzzleToBoloss"));

	if (actorInvestigate && FVector::Distance(actorInvestigate->GetActorLocation(), GetPawn()->GetActorLocation()) < 750.0f) {
		SetFocus(actorInvestigate);
		//GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 100;
	} else if (actorBoloss && FVector::Distance(actorBoloss->GetActorLocation(), GetPawn()->GetActorLocation()) < 750.0f) {
		SetFocus(actorBoloss);
		//GetCharacter()->GetCharacterMovement()->MaxWalkSpeed = 100;
	}
}

void AAIEnemyController::Wander() {

	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	FVector placeToInvestigate = BlackboardComponent->GetValueAsVector("PlaceToInvestigate");

	FVector location = UNavigationSystemV1::GetRandomReachablePointInRadius(GetWorld(), placeToInvestigate, 2 * LabBlock::assetSize);
	BlackboardComponent->SetValueAsVector("WanderPoint", location);
}

EPathFollowingRequestResult::Type AAIEnemyController::MoveToPriorityPoint()
{
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	AActor* target = Cast<AActor>(BlackboardComponent->GetValueAsObject("PriorityTargetPoint"));
	EPathFollowingRequestResult::Type res = EPathFollowingRequestResult::RequestSuccessful;
	if (target) {
		res = MoveToActor(target);
		if (res == EPathFollowingRequestResult::AlreadyAtGoal) {
			BlackboardComponent->ClearValue("PriorityTargetPoint");
			BlackboardComponent->SetValueAsVector("PlaceToInvestigate", GetPawn()->GetActorLocation());
		}
		return res;
	}
	return EPathFollowingRequestResult::Failed;
}

EPathFollowingRequestResult::Type AAIEnemyController::ChangeZone()
{
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	AActor* target = Cast<AActor>(BlackboardComponent->GetValueAsObject("NewZoneTargetPoint"));
	EPathFollowingRequestResult::Type res = EPathFollowingRequestResult::RequestSuccessful;
	if (target) {
		res = MoveToActor(target);
		if (res == EPathFollowingRequestResult::AlreadyAtGoal) {
			BlackboardComponent->ClearValue("NewZoneTargetPoint");
		}
		return res;
	}
	return EPathFollowingRequestResult::Failed;
}

void AAIEnemyController::ClearBlackboard()
{
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	if (BlackboardComponent->GetValueAsObject("NewZoneTargetPoint")) {
		BlackboardComponent->ClearValue("TargetActorToFollow");
		BlackboardComponent->ClearValue("PuzzleToBoloss");
		BlackboardComponent->ClearValue("PuzzleToInvestigate");
		BlackboardComponent->ClearValue("PuzzlePosition");
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
		BlackboardComponent->ClearValue("TargetPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("TargetActorToFollow")) {
		BlackboardComponent->ClearValue("PuzzleToBoloss");
		BlackboardComponent->ClearValue("PuzzleToInvestigate");
		BlackboardComponent->ClearValue("PuzzlePosition");
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
		BlackboardComponent->ClearValue("TargetPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("PuzzleToBoloss")) {
		BlackboardComponent->ClearValue("PuzzleToInvestigate");
		BlackboardComponent->ClearValue("PuzzlePosition");
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
		BlackboardComponent->ClearValue("TargetPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("PuzzleToInvestigate")) {
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
		BlackboardComponent->ClearValue("TargetPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("PriorityTargetPoint")) {
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
		BlackboardComponent->ClearValue("TargetPoint");
	}
	else if (BlackboardComponent->IsVectorValueSet("PlaceToInvestigate")) {
		BlackboardComponent->ClearValue("TargetPoint");
	}
}

void AAIEnemyController::UsePuzzle()
{
	Cast<ALabCharacter>(GetPawn())->Use();
}

void AAIEnemyController::FindPlayerToAttack()
{
	TArray<AActor*> players;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerCharacter::StaticClass(), players);
	UBlackboardComponent* bb = GetBrainComponent()->GetBlackboardComponent();

	for (AActor* p : players) {
		if (FVector::Dist(p->GetActorLocation(), GetPawn()->GetActorLocation()) < 100.0f) {  // DATA DRIVEEEEEEEEEEEEEEEEEEEEEEEEEEEEEN
			bb->SetValueAsObject("PlayerToAttack", p);
			return;
		}
	}
}
