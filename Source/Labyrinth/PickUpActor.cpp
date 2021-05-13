#include "PickUpActor.h"
#include "LabCharacter.h"
#include "kismet/GameplayStatics.h"

APickUpActor::APickUpActor()
{
	// Tous les objets qui peuvent être ramassés son simulés 
	// et peuvent être disséminés sur tout le niveau 
	MeshComp->SetSimulatePhysics(true);

	SetReplicateMovement(true);
}

void APickUpActor::Use(bool Event, APawn* InstigatorPawn)
{
	Super::Use(Event, InstigatorPawn);
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	//MeshComp->ToggleVisibility();
	savePos = GetActorLocation();
	SetActorLocation(savePos - FVector{ 0.f, 0.f, -550.f });
	//Destroy();
}

