#include "ChalkPickUpActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

AChalkPickUpActor::AChalkPickUpActor()
{
	ChalkMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChalkMesh"));

	ChalkMesh->SetupAttachment(MeshComp);
}

void AChalkPickUpActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		ChalkMesh->SetRenderCustomDepth(false);
}

void AChalkPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		ChalkMesh->SetRenderCustomDepth(true);
}

void AChalkPickUpActor::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);

	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
		if (IsValid(playerController) && !playerController->bHasChalk)
		{
			Super::Use(Event, InstigatorPawn);
			playerController->bHasChalk = true;
		}
	}

}
