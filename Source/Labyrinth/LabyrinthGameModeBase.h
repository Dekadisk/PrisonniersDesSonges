// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIDirector.h"
#include "GameFramework/PlayerController.h"
#include "LabyrinthGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALabyrinthGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
public:

	bool debug = false;

	ALabyrinthGameModeBase();

	TArray<AActor*> Starts;
	int currentIndex{};

	TArray<APlayerController*> WaitingPlayers;
public:
	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PostLogin(APlayerController* player) override;

	UFUNCTION(Exec)
	void ActivateDebug();

private:

	AAIDirector* AIdirector = nullptr;
	
};
