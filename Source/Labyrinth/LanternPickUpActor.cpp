// Fill out your copyright notice in the Description page of Project Settings.


#include "LanternPickUpActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

ALanternPickUpActor::ALanternPickUpActor()
{
	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));

	LanternMesh->SetupAttachment(MeshComp);
}

void ALanternPickUpActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		LanternMesh->SetRenderCustomDepth(false);
	}
}

void ALanternPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		LanternMesh->SetRenderCustomDepth(true);

	}
}

void ALanternPickUpActor::OnUsed(AActor* InstigatorActor)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorActor);
	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());
		if (IsValid(playerController) && !playerController->bHasLantern)
		{
			Super::OnUsed(InstigatorActor);
			playerController->bHasLantern = true;
		}
	}


}