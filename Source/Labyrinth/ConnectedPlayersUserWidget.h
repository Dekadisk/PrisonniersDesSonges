// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "Net/UnrealNetwork.h"
#include "ConnectedPlayersUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UConnectedPlayersUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(Replicated)
	TArray<FPlayerInfo> playersInfo;

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
