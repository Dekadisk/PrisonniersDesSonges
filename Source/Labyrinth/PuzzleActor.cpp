// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleActor.h"
#include <algorithm>
#include "Runtime/AIModule/Classes/Perception/AIPerceptionSystem.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"

// Sets default values
APuzzleActor::APuzzleActor()
{
	EPuzzleActions a= EPuzzleActions::Activate; // DELETE THIS
	targetActor.Add({});
}

// Called when the game starts or when spawned
void APuzzleActor::BeginPlay() {
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);

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
		std::for_each(targetActor.begin(), targetActor.end(), [&](FLinkedActors& link) {
			ExecuteAction(this, link.linkedActor, link.yes);
			});
	}
	else {
		std::for_each(targetActor.begin(), targetActor.end(), [&](FLinkedActors& link) {
			ExecuteAction(this, link.linkedActor, link.no);
			});
	}
}

int APuzzleActor::GetEtat()
{
	return 0;
}

void APuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();
}

void APuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

}