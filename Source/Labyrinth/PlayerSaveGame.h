// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "PlayerInfo.h"
#include "PlayerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UPlayerSaveGame : public USaveGame
{
	GENERATED_BODY()

private:

	UPROPERTY(EditDefaultsOnly, Category = "PlayerInfo", Replicated)
	FPlayerInfo pInfo;

public:

	UPlayerSaveGame();

	void SetPlayerInfo(FPlayerInfo info) { pInfo = info; }

	FPlayerInfo GetPlayerInfo() { return pInfo; }

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
