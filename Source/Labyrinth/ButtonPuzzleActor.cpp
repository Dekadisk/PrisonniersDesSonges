// Fill out your copyright notice in the Description page of Project Settings.


#include "ButtonPuzzleActor.h"
#include "PlayerCharacter.h"

AButtonPuzzleActor::AButtonPuzzleActor()
{
	MeshButtonBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverBase"));
	MeshButtonMovablePart = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverStick"));

	MeshButtonBase->SetupAttachment(MeshComp);
	MeshButtonMovablePart->SetupAttachment(MeshComp);
	isProcessing = false;
}

void AButtonPuzzleActor::Use(bool Event, APawn* InstigatorPawn)
{
	if (!isProcessing)
	{
		Super::Use(Event, InstigatorPawn);
		NetMulticastUseAnimation();
		CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
	}
}

void AButtonPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void AButtonPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshButtonBase->SetRenderCustomDepth(true);
		MeshButtonMovablePart->SetRenderCustomDepth(true);
	}
}

void AButtonPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshButtonBase->SetRenderCustomDepth(false);
		MeshButtonMovablePart->SetRenderCustomDepth(false);
	}
}

void AButtonPuzzleActor::NetMulticastUseAnimation_Implementation()
{
	UseAnimation();
}
