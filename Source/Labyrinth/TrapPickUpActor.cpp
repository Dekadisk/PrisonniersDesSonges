// Fill out your copyright notice in the Description page of Project Settings.


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
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage� 
		TrapMesh->SetRenderCustomDepth(false);
	}
}

void ATrapPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		TrapMesh->SetRenderCustomDepth(true);

	}
}

void ATrapPickUpActor::OnUsed(AActor* InstigatorActor)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorActor);
	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
		if (IsValid(playerController) && !playerController->bHasTrap)
		{
			Super::OnUsed(InstigatorActor);
			playerController->bHasTrap = true;
		}
	}
}
