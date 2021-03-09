// Fill out your copyright notice in the Description page of Project Settings.


#include "BellHintActor.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

ABellHintActor::ABellHintActor() {
	HintMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("HintMesh"));

	HintMesh->SetupAttachment(MeshComp);

	AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("AudioComp"));
	AudioComponent->bAutoActivate = false;
	AudioComponent->SetupAttachment(RootComponent);
	AudioComponent->OnAudioFinished.AddDynamic(this, &ABellHintActor::NextNote);
}

void ABellHintActor::NextNote()
{
	lastPlayed++;
	if (lastPlayed == NotesSounds.Num()) {
		lastPlayed = 0;
	}
	AudioComponent->SetSound(NotesSounds[lastPlayed]);
	if(lastPlayed != 0)
		AudioComponent->Play();
}

void ABellHintActor::BeginPlay()
{
	AudioComponent->SetSound(NotesSounds[0]);
}

void ABellHintActor::OnUsed(AActor* InstigatorActor)
{
	isProcessing = true;
	Animate();
	AudioComponent->Play();
	isProcessing = false;
}

void ABellHintActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		HintMesh->SetRenderCustomDepth(true);
	}
}

void ABellHintActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		HintMesh->SetRenderCustomDepth(false);
	}
}

void ABellHintActor::OnConstruction(const FTransform& Transform)
{
}
