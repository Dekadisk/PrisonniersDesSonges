// Fill out your copyright notice in the Description page of Project Settings.


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


void AClockPuzzleRoom::BeginPlay()
{
	Super::BeginPlay();
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

	FLinkedActors linkedClockActor;
	linkedClockActor.linkedActor = herseActor;
	linkedClockActor.no = EPuzzleActions::Lock;
	linkedClockActor.yes = EPuzzleActions::Unlock;

	FLinkedActors linkedLeverActor;
	linkedLeverActor.linkedActor = herseActor;
	linkedLeverActor.no = EPuzzleActions::None;
	linkedLeverActor.yes = EPuzzleActions::Activate;

	leverActor->targetActor.Add(linkedLeverActor);

	for (int clockId = 0; clockId < nbClocks; clockId++) {
		solutions.Add(HandDir(seed.GetUnsignedInt() % 8));//
		AClockPuzzleActor* clockActor = Cast<AClockPuzzleActor>(InstanceBP(TEXT("/Game/Blueprints/ClockPuzzleActor_BP.ClockPuzzleActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
		

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
		clockActor->targetActor.Add(linkedClockActor);
		do { clockActor->startPos = seed.GetUnsignedInt() % 8; } while (clockActor->startPos == clockActor->unlockPos);
		clockActor->currPos = clockActor->startPos;
		clockActor->OnConstruction({});
	}

	

}