#include "PickUpActor.h"

APickUpActor::APickUpActor()
{
	// Tous les objets qui peuvent �tre ramass�s son simul�s 
	// et peuvent �tre diss�min�s sur tout le niveau 
	MeshComp->SetSimulatePhysics(true);
	//bReplicates = true;
	SetReplicateMovement(true);
}

void APickUpActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Destroy();
}

