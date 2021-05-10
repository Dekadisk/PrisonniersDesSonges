#include "AIEnemyController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BrainComponent.h"
#include "AIEnemyTargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "MonsterCharacter.h"
#include <random>
#include "NavigationSystem.h"
#include "NavigationPath.h"
#include "DrawDebugHelpers.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "LabBlock.h"
#include "SolvableActor.h"
#include "Cachette.h"
#include "TrapActor.h"

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

	TArray<AActor*> tps;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AAIEnemyTargetPoint::StaticClass(), FName(FString::FromInt(currentSection)), tps);

	TArray<AAIEnemyTargetPoint*> candidates;
	for (int i = 0; i < 3; i++) {
		float min_dist = INFINITY;
		AActor* best = nullptr;
		for (AActor* tp : tps) {
			if (tp == TargetPoint || tp == PreviousTargetPoint) {
				continue;
			}
			float dist = FVector::Dist(PawnUsed->GetActorLocation(), tp->GetActorLocation());
			if (dist < min_dist) {
				min_dist = dist;
				best = tp;
			}
		}
		if (best) {
			auto path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), PawnUsed->GetActorLocation(), best);
			if (path->IsValid() && !path->IsPartial() && path->GetPathLength() < DataAsset->NavRadius) {
				candidates.Add(Cast<AAIEnemyTargetPoint>(best));
			}
			tps.Remove(best);
		}
	}

	if (candidates.Num() == 0) {
		BlackboardComponent->SetValueAsObject("TargetPoint", PreviousTargetPoint);
	}
	else {
		std::random_device rd;
		std::mt19937 prng{ rd() };
		std::uniform_int_distribution<int> tp_Rd{ 0, candidates.Num() - 1 };

		AAIEnemyTargetPoint* newTP;
		newTP = Cast<AAIEnemyTargetPoint>(candidates[tp_Rd(prng)]);

		PreviousTargetPoint = TargetPoint;
		BlackboardComponent->SetValueAsObject("TargetPoint", newTP);
	}
}

void AAIEnemyController::Sensing(const TArray<AActor*>& actors) {
	if (DataAsset->Sight) {
		for (AActor* actor : actors) {

			FActorPerceptionBlueprintInfo info;
			PerceptionComponent->GetActorsPerception(actor, info);

			UBlackboardComponent* blackboard = GetBrainComponent()->GetBlackboardComponent();

			APlayerCharacter* player = Cast<APlayerCharacter>(actor);
			ATrapActor* trap = Cast<ATrapActor>(actor);

			// actor is a player
			if (player) {

				AActor* PlayerActor = Cast<AActor>(blackboard->GetValueAsObject("TargetActorToFollow"));

				// IN SIGHT
				if (info.LastSensedStimuli[0].WasSuccessfullySensed())
					PlayerSeen(actor);

				// SIGHT LOST
				else if (actor == PlayerActor) {
					GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Now I don't !");

					ElementsInSight.Remove(actor);

					if (ElementsInSight.Num() != 0) {
						AActor** cachette = ElementsInSight.FindByPredicate([](const AActor* elem) {
							return Cast<ACachette>(elem);
							});

						if (cachette) {

							FVector playerPos = PlayerActor->GetActorLocation();
							FVector cachettePos = (*cachette)->GetActorLocation();

							if ((FVector{ playerPos.X, playerPos.Y, 0.f } - FVector{ cachettePos.X, cachettePos.Y, 0.f }).Size() < 70.f) {
								blackboard->SetValueAsObject("CachetteToDestroy", *cachette);
								blackboard->ClearValue("TargetActorToFollow");
								return;
							}
						}
					}

					PredictPlayerMvmt(PlayerActor);

				}

			}
			else if (!blackboard->GetValueAsObject("TargetActorToFollow")) {
				if (info.LastSensedStimuli[0].WasSuccessfullySensed()) {
					if (!ElementsInSight.Contains(actor))
						ElementsInSight.Add(actor);
					CheckElementChangedState(actor);
				}
			}
			else {
				if (info.LastSensedStimuli[0].WasSuccessfullySensed()) {
					if (!ElementsInSight.Contains(actor))
						ElementsInSight.Add(actor);
				}
			}

		}
	}
	else {
		UBlackboardComponent* blackboard = GetBrainComponent()->GetBlackboardComponent();
		blackboard->ClearValue("TargetActorToFollow");
		blackboard->ClearValue("CachetteToDestroy");
		blackboard->ClearValue("ObstacleToDestroy");
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

		AUsableActor* puzzle = Cast<AUsableActor>(actor);
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

	AUsableActor* puzzleInvestigate = Cast<AUsableActor>(actorInvestigate);
	if (puzzleInvestigate && !Cast<ASolvableActor>(actorInvestigate)) {
		if (puzzleInvestigate->GetEtat() != -1) {
			BlackboardComponent->ClearValue("PuzzleToInvestigate");
			BlackboardComponent->SetValueAsVector("PlaceToInvestigate", puzzleInvestigate->GetActorLocation());
			return;
		}

		TArray<ASolvableActor*> solvables;
		for (FPE_PuzzleEventMaster& pem : puzzleInvestigate->PuzzleEvents)
		{
			for (FPE_ActorInteractions& ai : pem.Event.Environment.ActorInteractions)
			{
				for (AUsableActor* a : ai.Actors) {
					if (ASolvableActor* sa = Cast<ASolvableActor>(a))
						if (!solvables.Contains(sa))
							solvables.Add(sa);
				}
			}
		}
		
		float dist_min = INFINITY;
		ASolvableActor* solvable = nullptr;
		for (ASolvableActor* sa : solvables) {
			UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), GetPawn()->GetActorLocation(), sa);

			if (!path->IsPartial()) {
				if (path->GetPathLength() < dist_min) {
					dist_min = path->GetPathLength();
					solvable = sa;
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
		if (solvable && solvable->isSolved && FCString::Atoi(*(solvable->Tags[0].ToString())) == currentSection) {
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
	if(GetFocusActor() != nullptr && FVector::Distance(GetFocusActor()->GetActorLocation(), GetPawn()->GetActorLocation()) < 300.0f)
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
		FVector loc = target->GetActorLocation();
		loc.Z = -100;
		res = MoveToLocation(loc);
		if (res == EPathFollowingRequestResult::AlreadyAtGoal) {
			BlackboardComponent->ClearValue("PriorityTargetPoint");
			BlackboardComponent->SetValueAsVector("PlaceToInvestigate", GetPawn()->GetActorLocation());
		}
		return res;
	}
	return EPathFollowingRequestResult::Failed;
}

EPathFollowingRequestResult::Type AAIEnemyController::MoveToPlayer()
{
	UBlackboardComponent* BlackboardComponent = BrainComponent->GetBlackboardComponent();
	AActor* target = Cast<AActor>(BlackboardComponent->GetValueAsObject("TargetActorToFollow"));
	EPathFollowingRequestResult::Type res = EPathFollowingRequestResult::RequestSuccessful;
	if (target) {
		res = MoveToActor(target, 40.0f);
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
		if (res == EPathFollowingRequestResult::AlreadyAtGoal)
			BlackboardComponent->ClearValue("NewZoneTargetPoint");

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
	}
	else if (BlackboardComponent->GetValueAsObject("TargetActorToFollow")) {
		BlackboardComponent->ClearValue("PuzzleToBoloss");
		BlackboardComponent->ClearValue("PuzzleToInvestigate");
		BlackboardComponent->ClearValue("PuzzlePosition");
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("PuzzleToBoloss")) {
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("PuzzleToInvestigate")) {
		BlackboardComponent->ClearValue("PriorityTargetPoint");
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
	}
	else if (BlackboardComponent->GetValueAsObject("PriorityTargetPoint")) {
		BlackboardComponent->ClearValue("PlaceToInvestigate");
		BlackboardComponent->ClearValue("WanderPoint");
	}
	else if (BlackboardComponent->IsVectorValueSet("PlaceToInvestigate")) {
	}
}

void AAIEnemyController::UsePuzzle()
{
	Cast<ALabCharacter>(GetPawn())->Use();
	UBlackboardComponent* bb = GetBrainComponent()->GetBlackboardComponent();
	AUsableActor* puzzle = Cast<AUsableActor>(bb->GetValueAsObject("PuzzleToBoloss"));
	PuzzlesInMemory[puzzle] = puzzle->GetEtat();
	bb->ClearValue("PuzzleToBoloss");
	bb->ClearValue("PuzzleToInvestigate");
}

void AAIEnemyController::AttackPlayer()
{
	UBlackboardComponent* bb = GetBrainComponent()->GetBlackboardComponent();
	APlayerCharacter* Target = Cast<APlayerCharacter>(bb->GetValueAsObject("TargetActorToFollow"));
	if (Target) {
		AMonsterCharacter* MyPawn = Cast<AMonsterCharacter>(GetPawn());
		MyPawn->MulticastAttackPlayer(Target);
		bb->ClearValue("TargetActorToFollow");
		GetBrainComponent()->StopLogic("Animation");
	}
}

void AAIEnemyController::StartHunt()
{
	UBlackboardComponent* bb = GetBrainComponent()->GetBlackboardComponent();

	APlayerCharacter* Target = Cast<APlayerCharacter>(bb->GetValueAsObject("TargetActorToFollow"));
	if (Target) {
		AMonsterCharacter* MyPawn = Cast<AMonsterCharacter>(GetPawn());
		MyPawn->MulticastStartHunt(Target);
	}
}

void AAIEnemyController::DestroyCachette() {

	UBlackboardComponent* bb = GetBrainComponent()->GetBlackboardComponent();
	ACachette* cachette = Cast<ACachette>(bb->GetValueAsObject("CachetteToDestroy"));
	if (cachette) {
		AMonsterCharacter* MyPawn = Cast<AMonsterCharacter>(GetPawn());
		MyPawn->MulticastAttackCachette(cachette);
		bb->ClearValue("CachetteToDestroy");
		GetBrainComponent()->StopLogic("Animation");
	}
}

void AAIEnemyController::DestroyTrap() {

	UBlackboardComponent* bb = GetBrainComponent()->GetBlackboardComponent();
	ATrapActor* trap = Cast<ATrapActor>(bb->GetValueAsObject("ObstacleToDestroy"));
	if (trap) {
		AMonsterCharacter* MyPawn = Cast<AMonsterCharacter>(GetPawn());
		MyPawn->MulticastDestroyTrap(trap);
		bb->ClearValue("ObstacleToDestroy");
	}
}

void AAIEnemyController::PlayerSeen(AActor* player) {

	GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "Now I see you!");

	UBlackboardComponent* blackboard = GetBrainComponent()->GetBlackboardComponent();
	AActor* PlayerActor = Cast<AActor>(blackboard->GetValueAsObject("TargetActorToFollow"));

	ElementsInSight.Add(player);

	FVector newSeenPos = player->GetActorLocation();
	UNavigationPath* path2 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), newSeenPos, this);

	if (PlayerActor != nullptr) {
		FVector currentTargetPos = PlayerActor->GetActorLocation();
		UNavigationPath* path1 = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), currentTargetPos, this);

		if (path1->IsValid() && !path1->IsPartial() && path2->IsValid() && !path2->IsPartial()) {
			if (path1->GetPathLength() > path2->GetPathLength())
				blackboard->SetValueAsObject("TargetActorToFollow", player);
		}
	}
	else {
		if (path2->IsValid() && !path2->IsPartial())
			blackboard->SetValueAsObject("TargetActorToFollow", player);
	}
}

void AAIEnemyController::PredictPlayerMvmt(AActor* PlayerActor) {

	UBlackboardComponent* blackboard = GetBrainComponent()->GetBlackboardComponent();

	blackboard->ClearValue("TargetActorToFollow");
	float dist_min = INFINITY;
	AAIEnemyTargetPoint* point1 = nullptr;
	AAIEnemyTargetPoint* point2 = nullptr;

	float pathLength1;
	float pathLength2;

	TArray<AActor*> tps;
	UGameplayStatics::GetAllActorsOfClassWithTag(GetWorld(), AAIEnemyTargetPoint::StaticClass(), FName(FString::FromInt(currentSection)), tps);
	for (AActor* tp : tps) {

		UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), tp->GetActorLocation(), PlayerActor);

		if (path->IsValid() && !path->IsPartial()) {
			if (path->GetPathLength() < dist_min) {
				dist_min = path->GetPathLength();
				point1 = Cast<AAIEnemyTargetPoint>(tp);
			}
		}

	}

	if (point1 != nullptr) {
		pathLength1 = dist_min;
		dist_min = INFINITY;

		for (AActor* tp : tps) {

			if (point1->GetName() != tp->GetName()) {
				UNavigationPath* path = UNavigationSystemV1::FindPathToActorSynchronously(GetWorld(), tp->GetActorLocation(), PlayerActor);

				if (path->IsValid() && !path->IsPartial()) {
					if (path->GetPathLength() < dist_min) {
						dist_min = path->GetPathLength();
						point2 = Cast<AAIEnemyTargetPoint>(tp);
					}
				}
			}
		}
		pathLength2 = dist_min;

		// DEBUG LINE
		DrawDebugLine(GetWorld(), point1->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, point2->GetActorLocation() + FVector{ 0.0f,0.0f,20.0f }, FColor{ 255,0,0 }, false, 5.f, (uint8)'\000', 10.f);

		if (pathLength1 < pathLength2)
			blackboard->SetValueAsObject("PriorityTargetPoint", point2);
		else
			blackboard->SetValueAsObject("PriorityTargetPoint", point1);
	}
}