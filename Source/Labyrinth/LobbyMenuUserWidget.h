// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInfo.h"
#include "Components/VerticalBox.h"
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
	void UpdateSeedDisplay(int seed);

	UFUNCTION(Category = "Lobby")
	void UpdateStatus();

	//OnClick Function
	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickLeaveLobby();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickReadyStart();

	//Bind Function
	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindServerName();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindPlayerDisplay();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindReadyButtonText();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindSeedDisplay();

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(BlueprintReadOnly, Category = "WidgetCPP")
	UVerticalBox *PlayerWindow;

private:

	UPROPERTY(Transient, Replicated)
	FText ServerName;

	UPROPERTY(Transient, Replicated)
	FText PlayersDisplay;

	UPROPERTY()
	FText ReadyButtonText;

	UPROPERTY(Transient, Replicated)
	FText SeedDisplay;

	UPROPERTY()
	class ALobbyPlayerController *PlayerOwner;

	

};