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

	UPROPERTY(BlueprintReadWrite, Replicated, meta=(ExposeOnSpawn="true"))
	FPlayerInfo playersInfo;

	UPROPERTY(BlueprintReadWrite, Replicated, meta=(ExposeOnSpawn="true"))
	int id;

	UFUNCTION(BlueprintCallable, Category = "Kick")
	void OnClickKick();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindStatus();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindPlayerName();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	bool BindKickVisibility();

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
