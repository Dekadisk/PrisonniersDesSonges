// Fill out your copyright notice in the Description page of Project Settings.


#include "KeyPickUpActor.h"
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
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		KeyMesh->SetRenderCustomDepth(true);

	}
}
