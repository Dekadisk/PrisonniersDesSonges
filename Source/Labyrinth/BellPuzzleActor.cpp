// Fill out your copyright notice in the Description page of Project Settings.


#include "BellPuzzleActor.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"
#include "DrawDebugHelpers.h"

ABellPuzzleActor::ABellPuzzleActor()
{
	Bell = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshBell"));
	BellStick = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshBellArm"));

	Bell->SetupAttachment(MeshComp);
	BellStick->SetupAttachment(MeshComp);

	static ConstructorHelpers::FObjectFinder<USoundWave> bellSoundWave(TEXT("/Game/Assets/Audio/Bell/bellSound.bellSound"));
	NoteSound = bellSoundWave.Object;
}

void ABellPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	UGameplayStatics::PlaySoundAtLocation(this, NoteSound, GetActorLocation(), 1.0F, note/6.0F*2.0F);/*1.0F/(GetActorScale3D().X/5.0F*/

	ProcessTargetActions(true);

	Bell->UPrimitiveComponent::AddImpulse(InstigatorActor->GetActorForwardVector() * 5000, FName("DEF_PENDULUM"), false);
	Bell->UPrimitiveComponent::AddImpulse(InstigatorActor->GetActorForwardVector() * 6000, FName("DEF_SHELL"), false);

	DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + InstigatorActor->GetActorForwardVector() * 100, FColor::Blue, true);
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

void ABellPuzzleActor::BeginPlay() {
	Super::BeginPlay();
}

void ABellPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
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
