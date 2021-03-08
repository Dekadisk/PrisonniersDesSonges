// Fill out your copyright notice in the Description page of Project Settings.


#include "BellPuzzleActor.h"
#include "Kismet/GameplayStatics.h"

ABellPuzzleActor::ABellPuzzleActor()
{
	Bell = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Bell"));
	BellStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BellStick"));

	Bell->AttachTo(MeshComp);
	BellStick->AttachTo(MeshComp);
}

void ABellPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	UGameplayStatics::PlaySoundAtLocation(this, NoteSound, GetActorLocation());

	isProcessing = true;
	Animate();
	isProcessing = false;

	ProcessTargetActions(true);
}

void ABellPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		Bell->SetRenderCustomDepth(true);
	}
}

void ABellPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		Bell->SetRenderCustomDepth(true);
	}
}

void ABellPuzzleActor::OnConstruction(const FTransform& Transform)
{
	/*switch (note)
		case 1:

		case 2:

		case 3:

		case 4:

		case 5:

		case 6:*/
}
