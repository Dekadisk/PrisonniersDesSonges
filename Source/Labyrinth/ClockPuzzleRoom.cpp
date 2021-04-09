#include "ClockPuzzleRoom.h"
#include "LeverPuzzleActor.h"
#include "ClockPuzzleActor.h"
#include "ClockDoorSolvableActor.h"

AClockPuzzleRoom::AClockPuzzleRoom() {
	nbClocks = 4;
	solutions.Add(HandDir::UP);
	solutions.Add(HandDir::RIGHT);
	solutions.Add(HandDir::DOWN);
	solutions.Add(HandDir::LEFT);
}

void AClockPuzzleRoom::InitPuzzle(FRandomStream seed)
{
	// Add Lever and Herse
	ALeverPuzzleActor* leverActor = Cast<ALeverPuzzleActor>(InstanceBP(TEXT("/Game/Blueprints/LeverPuzzleActor_BP.LeverPuzzleActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	AClockDoorSolvableActor* herseActor = Cast<AClockDoorSolvableActor>(InstanceBP(TEXT("/Game/Blueprints/ClockDoorSolvableActor_BP.ClockDoorSolvableActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	leverActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Lever0"));
	herseActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Herse0"));

	herseActor->unlockLimit = nbClocks;
	//Add clocks
	solutions.Empty();
	randomSeed = seed;

	FPE_ActorInteractions ailever;
	ailever.Actors.Add(herseActor);
	ailever.Interaction = EPuzzleEventInteraction::Activate;
	FPE_Environment evlever;
	evlever.ActorInteractions.Add(ailever);
	FPE_PuzzleEvent pelever;
	pelever.Environment = evlever;
	FPE_PuzzleEventMaster emlever;
	emlever.Event = pelever;
	emlever.Trigger = EPuzzleEventCheck::On;
	leverActor->PuzzleEvents.Add(emlever);

	for (int clockId = 0; clockId < nbClocks; clockId++) {
		solutions.Add(HandDir(seed.GetUnsignedInt() % 8));//
		AClockPuzzleActor* clockActor = Cast<AClockPuzzleActor>(InstanceBP(TEXT("/Game/Blueprints/ClockPuzzleActor_BP.ClockPuzzleActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
		
		FPE_ActorInteractions ai;
		ai.Actors.Add(herseActor);
		ai.Interaction = EPuzzleEventInteraction::Unlock;
		FPE_Environment ev;
		ev.ActorInteractions.Add(ai);
		FPE_PuzzleEvent pe;
		pe.Environment = ev;
		FPE_PuzzleEventMaster em;
		em.Event = pe;
		em.Trigger = EPuzzleEventCheck::Unlock;
		clockActor->PuzzleEvents.Add(em);

		FPE_ActorInteractions ai2;
		ai2.Actors.Add(herseActor);
		ai2.Interaction = EPuzzleEventInteraction::Lock;
		FPE_Environment ev2;
		ev2.ActorInteractions.Add(ai2);
		FPE_PuzzleEvent pe2;
		pe2.Environment = ev2;
		FPE_PuzzleEventMaster em2;
		em2.Event = pe2;
		em2.Trigger = EPuzzleEventCheck::Lock;
		clockActor->PuzzleEvents.Add(em2);

		switch (clockId)
		{
		case 0:
			clockActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Clock0"));
			break;
		case 1:
			clockActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Clock1"));
			break;
		case 2:
			clockActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Clock2"));
			break;
		case 3:
			clockActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Clock3"));
			break;
		default:
			break;
		}
		clockActor->unlockPos = int(solutions.Last());
		do { clockActor->startPos = seed.GetUnsignedInt() % 8; } while (clockActor->startPos == clockActor->unlockPos);
		clockActor->currPos = clockActor->startPos;
		clockActor->OnConstruction({});

		herseActor->elements.Add(clockActor);
	}	

}