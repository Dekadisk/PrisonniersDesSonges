// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverPuzzleActor.h"
#include <Labyrinth/LabCharacter.h>

// Called every frame
ALeverPuzzleActor::ALeverPuzzleActor()
{
	MeshLeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverBase"));
	MeshLeverStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverStick"));

	MeshLeverBase->AttachTo(MeshComp);
	MeshLeverStick->AttachTo(MeshComp);
}

void ALeverPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	ALabCharacter* MyCharacter = Cast<ALabCharacter>(InstigatorActor);
	if (MyCharacter)
	{
		isEnable = !isEnable;
	}
}

void ALeverPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshLeverBase->SetRenderCustomDepth(true);
		MeshLeverStick->SetRenderCustomDepth(true);
	}
}

void ALeverPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshLeverBase->SetRenderCustomDepth(false);
		MeshLeverStick->SetRenderCustomDepth(false);
	}
}