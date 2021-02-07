// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleActor.h"
#include <algorithm>

// Sets default values
APuzzleActor::APuzzleActor()
{
	EPuzzleActions a= EPuzzleActions::Activate; // DELETE THIS

	targetActor.Add({});
}

// Called when the game starts or when spawned
void APuzzleActor::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void APuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APuzzleActor::OnUsed(AActor* InstigatorActor)
{
	// Rien ici, les classes d�riv�es s'en occuperont 
}

void APuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}

void APuzzleActor::ProcessTargetActions(bool b)
{
	if (b) {
		std::for_each(targetActor.begin(), targetActor.end(), [](FLinkedActors& link) {
			ExecuteAction(link.linkedActor, link.yes);
			});
	}
	else {
		std::for_each(targetActor.begin(), targetActor.end(), [](FLinkedActors& link) {
			ExecuteAction(link.linkedActor, link.no);
			});
	}
}

void APuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();
}

void APuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

}