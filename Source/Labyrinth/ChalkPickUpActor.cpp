// Fill out your copyright notice in the Description page of Project Settings.


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
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage� 
		ChalkMesh->SetRenderCustomDepth(false);
	}
}

void AChalkPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		ChalkMesh->SetRenderCustomDepth(true);

	}
}

void AChalkPickUpActor::OnUsed(AActor* InstigatorActor)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorActor);
	
	

	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
		if (IsValid(playerController) && !playerController->bHasChalk)
		{
			Super::OnUsed(InstigatorActor);
			playerController->bHasChalk = true;
		}
	}


}