#include "PickUpActor.h"
#include "LabCharacter.h"
#include "kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "LabyrinthPlayerController.h"

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
	Cast<ALabyrinthPlayerController>(Cast<APlayerCharacter>(InstigatorPawn)->GetController())->PlayMusic(PickupSound);

	//MeshComp->ToggleVisibility();
	savePos = GetActorLocation();
	SetActorLocation(savePos - FVector{ 0.f, 0.f, -550.f });
	//Destroy();
}

