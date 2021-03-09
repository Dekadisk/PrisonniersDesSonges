// Fill out your copyright notice in the Description page of Project Settings.


#include "LanternPickUpActor.h"

#include "LabCharacter.h"

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
	if (IsValid(player) && !player->bHasKey)
	{
		Super::OnUsed(InstigatorActor);
		player->bHasLantern = true;
	}


}