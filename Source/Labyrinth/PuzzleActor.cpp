// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleActor.h"

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
	// Rien ici, les classes dérivées s'en occuperont 
}

void APuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();
}

void APuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

}