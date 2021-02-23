// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LabyrinthGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALabyrinthGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:
	ALabyrinthGameModeBase();

	TArray<AActor*> Starts;
	int currentIndex{};
public:
	AActor* ChoosePlayerStart_Implementation(AController* Player) override;
};
