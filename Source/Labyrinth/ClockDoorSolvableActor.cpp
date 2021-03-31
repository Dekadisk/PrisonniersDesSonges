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
	if (isSolved && unlockCount < unlockLimit) {
		isSolved = false;
		Animate();
	}
		
}

void AClockDoorSolvableActor::Activate()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Activate"));
	}
	if (!isSolved && unlockCount >= unlockLimit) {
		isSolved = true;
		Animate();
	}

}

void AClockDoorSolvableActor::Open()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Open"));
	}
	if (!isSolved) {
		isSolved = true;
		Animate();
	}

}

void AClockDoorSolvableActor::Close()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Close"));
	}
	if (isSolved) {
		isSolved = false;
		Animate();
	}
}

void AClockDoorSolvableActor::OnConstruction(const FTransform& Transform)
{
	isSolved = false;
}
