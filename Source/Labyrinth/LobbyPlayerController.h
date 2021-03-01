// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PlayerInfo.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ALobbyPlayerController();

	UFUNCTION(Reliable, Client)
	void OnRep_InitialSetup();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerCallUpdate(FPlayerInfo info, bool ChangedStatus);

	UFUNCTION(Reliable, Client)
	void OnRep_SetupLobbyMenu(FText ServerName);

	void SaveGameCheck();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Transient, Replicated)
	FPlayerInfo playerSettings;

	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;
	
};
