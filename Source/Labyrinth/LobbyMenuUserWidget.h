// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInfo.h"
#include "LobbyMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ULobbyMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnConstructLobby(); 

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void ClearPlayerList();

	UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Lobby")
	void UpdatePlayerWindow(FPlayerInfo playerInfo);

	UFUNCTION(Category = "Lobby")
	void UpdatePlayersDisplay(int currentNumberPlayer);

	UFUNCTION(Category = "Lobby")
	void UpdateStatus();


	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickLeaveLobby();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickReadyStart();

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:

	UPROPERTY(Transient, Replicated)
	FText ServerName;

	UPROPERTY(Transient, Replicated)
	FText PlayersDisplay;

	UPROPERTY()
	class ALobbyPlayerController *PlayerOwner;

	UPROPERTY()
	bool isServer;

	UPROPERTY()
	FText ReadyButtonText;

};
