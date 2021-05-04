#include "BellDoorSolvableActor.h"

ABellDoorSolvableActor::ABellDoorSolvableActor()
{
	waited.Push(1);
	waited.Push(2);
	waited.Push(3);
	waited.Push(4);
}

void ABellDoorSolvableActor::Ring(bool Event, int32 note)
{
	//if(GEngine)
	//	GEngine->AddOnScreenDebugMessage(-1, 1.1f, FColor::Orange, TEXT("Ring"));

	playing.Push(note);

	if (playing.Num() > waited.Num())
		playing.RemoveAt(0);

	if (waited == playing) {
		isSolved = true;
		//if (GEngine)
		//	GEngine->AddOnScreenDebugMessage(-1, 1.1f, FColor::Green, TEXT("Solved"));
		Animate();
	}
}

void ABellDoorSolvableActor::OnRep_Solvable()
{
	Super::OnRep_Solvable();
	Animate();
}

int ABellDoorSolvableActor::GetEtat() {
	return isSolved ? -1 : 0;
}