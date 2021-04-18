#include "SolvableActor.h"

// Sets default values
ASolvableActor::ASolvableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	bDisableFocus = true;
}

void ASolvableActor::OnRep_Solvable()
{
}

void ASolvableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASolvableActor, isSolved);
}