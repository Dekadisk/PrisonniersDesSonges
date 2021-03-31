#include "PuzzleActor.h"
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
	Super::BeginPlay();

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}

void APuzzleActor::OnUsed(AActor* InstigatorActor)
{
	// Rien ici, les classes d�riv�es s'en occuperont 
}

void APuzzleActor::ProcessTargetActions(bool success)
{
	if (success) {
		for (FLinkedActors link : targetActor)
			ExecuteAction(this, link.linkedActor, link.yes);
	}
	else {
		for (FLinkedActors link : targetActor)
			ExecuteAction(this, link.linkedActor, link.no);
	}
}

int APuzzleActor::GetEtat()
{
	return 0;
}