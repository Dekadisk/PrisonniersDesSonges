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
	void ClearPlayerList();

	/*UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnRep_UpdatePlayerWindow(FPlayerInfo info);

	UPROPERTY(Transient, ReplicatedUsing = OnRep_UpdatePlayerWindow)
	TArray<FPlayerInfo> infos;

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;*/

};
