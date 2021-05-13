// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "CutsceneGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ACutsceneGameModeBase : public AGameModeBase
{
	GENERATED_BODY()
	
public:
	ACutsceneGameModeBase();

	void GenericPlayerInitialization(AController* player) override;

	UFUNCTION(BlueprintCallable)
	void EndGame();

	UPROPERTY()
	TArray<APlayerController*> AllPlayerControllers;
};
