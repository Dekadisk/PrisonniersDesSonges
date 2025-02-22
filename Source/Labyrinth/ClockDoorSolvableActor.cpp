#include "ClockDoorSolvableActor.h"

void AClockDoorSolvableActor::Unlock(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Unlock"));

	unlockCount++;
}

void AClockDoorSolvableActor::Lock(bool Event)
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

void AClockDoorSolvableActor::Activate(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Activate"));

	if (!isSolved && unlockCount >= unlockLimit) {
		isSolved = true;
		Animate();
	}

}

void AClockDoorSolvableActor::On(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Open"));

	if (!isSolved) {
		isSolved = true;
		Animate();
	}

}

void AClockDoorSolvableActor::Off(bool Event)
{
	//if (GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Orange, TEXT("Close"));

	if (isSolved) {
		isSolved = false;
		Animate();
	}
}

void AClockDoorSolvableActor::OnConstruction(const FTransform& Transform)
{
	isSolved = false;
}

void AClockDoorSolvableActor::OnRep_Solvable()
{
	Super::OnRep_Solvable();
	Animate();
}

int AClockDoorSolvableActor::GetEtat() {
	return isSolved ? -1 : 0;
}
