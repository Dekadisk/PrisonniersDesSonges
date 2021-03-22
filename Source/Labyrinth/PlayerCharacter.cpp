// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"
#include <Runtime/AIModule/Classes/Perception/AIPerceptionSystem.h>
#include <Runtime/AIModule/Classes/Perception/AISense_Sight.h>
#include <Kismet/GameplayStatics.h>
#include "LabyrinthGameModeBase.h"


void APlayerCharacter::BeginPlay() {
	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);

	Super::BeginPlay();
}

