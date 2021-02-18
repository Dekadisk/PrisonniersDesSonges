// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockDoorSolvableActor.h"


AClockDoorSolvableActor::AClockDoorSolvableActor() {
	SetMobility(EComponentMobility::Movable);
}

void AClockDoorSolvableActor::Unlock()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Unlock"));
	}
	unlockCount++;
}

void AClockDoorSolvableActor::Lock()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Lock"));
	}
	if (unlockCount > 0)
		unlockCount--;
	if (isOpened && unlockCount < unlockLimit) {
		isOpened = false;
		Animate();
	}
		
}

void AClockDoorSolvableActor::Activate()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Activate"));
	}
	if (!isOpened && unlockCount >= unlockLimit) {
		isOpened = true;
		Animate();
	}

}

void AClockDoorSolvableActor::Open()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Open"));
	}
	if (!isOpened) {
		isOpened = true;
		Animate();
	}

}

void AClockDoorSolvableActor::Close()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Close"));
	}
	if (isOpened) {
		isOpened = false;
		Animate();
	}
}

void AClockDoorSolvableActor::OnConstruction(const FTransform& Transform)
{
	isOpened = false;
}
