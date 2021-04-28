#include "TrapPickUpActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

ATrapPickUpActor::ATrapPickUpActor()
{
	TrapMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrapMesh"));

	TrapMesh->SetupAttachment(MeshComp);
}

void ATrapPickUpActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		TrapMesh->SetRenderCustomDepth(false);
	}
}

void ATrapPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		TrapMesh->SetRenderCustomDepth(true);

	}
}

void ATrapPickUpActor::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
		if (IsValid(playerController) && !playerController->bHasTrap)
		{
			Super::Use(Event, InstigatorPawn);
			playerController->bHasTrap = true;
		}
	}
}
