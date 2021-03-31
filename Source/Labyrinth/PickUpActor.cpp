#include "PickUpActor.h"

APickUpActor::APickUpActor()
{
	// Tous les objets qui peuvent être ramassés son simulés 
	// et peuvent être disséminés sur tout le niveau 
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

