// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInfo.h"
#include "LobbyMenuUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "LobbyPlayerController.generated.h"


/**
 * 
 */
UCLASS()
class LABYRINTH_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()
//
public:

	ALobbyPlayerController();

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void InitialSetup();

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void AddPlayerInfo(const TArray<FPlayerInfo> &connectedPlayerInfo);

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void UpdateLocalSettings(int seed);

	UFUNCTION(Server, Reliable, WithValidation, Category = "PCLobby")
	void ServerCallUpdate(FPlayerInfo info, bool isReady);

	UFUNCTION(Reliable, Client)
	void SetupLobbyMenu(const FText &ServerName);

	void SaveGameCheck();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Replicated)
	FPlayerInfo playerSettings;

	UPROPERTY(Transient, Replicated)
	bool ready;

	UPROPERTY(Transient, Replicated)
	TArray<FPlayerInfo> allConnectedPlayers;

	UPROPERTY()
	ULobbyMenuUserWidget* LobbyMenu;

	TOptional<int> partySeed;

	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;
	
};
