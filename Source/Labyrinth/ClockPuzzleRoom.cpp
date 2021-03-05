// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockPuzzleRoom.h"
#include "ClockPuzzleActor.h"

AClockPuzzleRoom::AClockPuzzleRoom() {
	nbClocks = 4;
	solutions.Add(HandDir::UP);
}


void AClockPuzzleRoom::BeginPlay()
{
	Super::BeginPlay();
}

void AClockPuzzleRoom::InitPuzzle(FRandomStream seed)
{
	solutions.Empty();
	randomSeed = seed;
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
		clockActor->unlockPos = seed.GetUnsignedInt() % 8;
		
		do { clockActor->startPos = seed.GetUnsignedInt() % 8; } while (clockActor->startPos == clockActor->unlockPos);
		clockActor->currPos = clockActor->startPos;
		clockActor->OnConstruction({});
	}

}