#include "PlayerCharacter.h"
#include <Runtime/AIModule/Classes/Perception/AIPerceptionSystem.h>
#include <Runtime/AIModule/Classes/Perception/AISense_Sight.h>


void APlayerCharacter::BeginPlay() {
	Super::BeginPlay();

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);
}