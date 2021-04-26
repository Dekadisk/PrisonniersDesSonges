#include "PickUpActor.h"
#include "kismet/GameplayStatics.h"

APickUpActor::APickUpActor()
{
	// Tous les objets qui peuvent �tre ramass�s son simul�s 
	// et peuvent �tre diss�min�s sur tout le niveau 
	MeshComp->SetSimulatePhysics(true);

	SetReplicateMovement(true);
}

void APickUpActor::Use(bool Event, APawn* InstigatorPawn)
{
	Super::Use(Event, InstigatorPawn);
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Destroy();
}

