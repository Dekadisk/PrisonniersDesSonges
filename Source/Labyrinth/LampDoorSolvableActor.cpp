// Fill out your copyright notice in the Description page of Project Settings.


#include "LampDoorSolvableActor.h"


void ALampDoorSolvableActor::Unlock(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Unlock"));

	unlockCount++;
	if (!isSolved && unlockCount == unlockLimit) {
		isSolved = true;
		Animate();
	}
}

void ALampDoorSolvableActor::Lock(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lock"));

	if (unlockCount > 0)
		unlockCount--;

	if (isSolved && unlockCount < unlockLimit) {
		isSolved = false;
		Animate();
	}

}

void ALampDoorSolvableActor::Activate(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Activate"));

	if (!isSolved && unlockCount >= unlockLimit) {
		isSolved = true;
		Animate();
	}

}

void ALampDoorSolvableActor::On(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Open"));

	if (!isSolved) {
		isSolved = true;
		Animate();
	}

}

void ALampDoorSolvableActor::Off(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Close"));

	if (isSolved) {
		isSolved = false;
		Animate();
	}
}

void ALampDoorSolvableActor::OnConstruction(const FTransform& Transform)
{
	isSolved = false;
}

int ALampDoorSolvableActor::GetEtat() {
	return isSolved ? -1 : 0;
}
