#include "LeverPuzzleActor.h"
#include <Labyrinth/PlayerCharacter.h>

ALeverPuzzleActor::ALeverPuzzleActor()
{
	MeshLeverBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverBase"));
	MeshLeverStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshLeverStick"));

	MeshLeverBase->SetupAttachment(MeshComp);
	MeshLeverStick->SetupAttachment(MeshComp);
	isEnable = false;
	isProcessing = false; 
}

void ALeverPuzzleActor::Use(bool Event, APawn* InstigatorPawn)
{
	if (!isProcessing)
	{
		Super::Use(Event, InstigatorPawn);
		APlayerCharacter* MyCharacter = Cast<APlayerCharacter>(InstigatorPawn);
		if (MyCharacter)
		{
			isEnable = !isEnable;
		}
		isEnable ? EnableAnimation() : DisableAnimation();
		CheckEvents(EPuzzleEventCheck::On, InstigatorPawn);
	}
}

void ALeverPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALeverPuzzleActor, isEnable);
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

void ALeverPuzzleActor::OnRep_EnableDisableAnimation()
{
	isEnable ? EnableAnimation() : DisableAnimation();
}

int ALeverPuzzleActor::GetEtat() {
	return isEnable ? -1 : 0;
}