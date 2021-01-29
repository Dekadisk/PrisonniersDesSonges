// Fill out your copyright notice in the Description page of Project Settings.


#include "LeverPuzzleActor.h"
#include <Labyrinth/LabCharacter.h>

// Called every frame
ALeverPuzzleActor::ALeverPuzzleActor()
{
	MeshLeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverBase"));
	MeshLeverStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverStick"));

	MeshLeverBase->SetupAttachment(MeshComp);
	MeshLeverStick->SetupAttachment(MeshComp);
}

void ALeverPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALeverPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	if (HasAuthority())
	{
		ServerOnUsed(InstigatorActor);
	}
	else
	{
		ALabCharacter* MyCharacter = Cast<ALabCharacter>(InstigatorActor);
		if (MyCharacter)
		{
			isEnable ? EnableAnimation() : DisableAnimation();
			isEnable = !isEnable;
		}
	}
}

void ALeverPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALeverPuzzleActor, isEnable);
}

void ALeverPuzzleActor::ServerOnUsed_Implementation(AActor* InstigatorActor)
{
	//Super::ServerOnUsed_Implementation(InstigatorActor);
	ALabCharacter* MyCharacter = Cast<ALabCharacter>(InstigatorActor);
	if (MyCharacter)
	{
		isEnable ? EnableAnimation() : DisableAnimation();
		isEnable = !isEnable;
	}
}

bool ALeverPuzzleActor::ServerOnUsed_Validate(AActor* InstigatorActor)
{
	return /*Super::ServerOnUsed_Validate(InstigatorActor) &&*/ true;
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