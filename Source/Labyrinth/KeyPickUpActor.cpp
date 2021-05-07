#include "KeyPickUpActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

AKeyPickUpActor::AKeyPickUpActor() 
{
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));

	KeyMesh->SetupAttachment(MeshComp);
}

void AKeyPickUpActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		KeyMesh->SetRenderCustomDepth(false);
	}
}

void AKeyPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		KeyMesh->SetRenderCustomDepth(true);
}

void AKeyPickUpActor::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
		if (IsValid(playerController) && !playerController->bHasKey)
		{
			Super::Use(Event, InstigatorPawn);
			playerController->bHasKey = true;
		}
		
	}
}
