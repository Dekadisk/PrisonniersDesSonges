// Fill out your copyright notice in the Description page of Project Settings.


#include "BellPuzzleActor.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

ABellPuzzleActor::ABellPuzzleActor()
{
	Bell = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshBell"));
	BellStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBellArm"));

	Bell->SetupAttachment(MeshComp);
	BellStick->SetupAttachment(MeshComp);
}

void ABellPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	UGameplayStatics::PlaySoundAtLocation(this, NoteSound, GetActorLocation());

	ProcessTargetActions(true);

	Bell->UPrimitiveComponent::AddImpulse(InstigatorActor->GetActorForwardVector() * 5000, FName("DEF_PENDULUM"), false);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + InstigatorActor->GetActorForwardVector() * 100, FColor::Blue, true);
}

void ABellPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		Bell->SetRenderCustomDepth(true);
	}
}

void ABellPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		Bell->SetRenderCustomDepth(false);
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