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
	FPlayerInfo playersInfo;

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindStatus();

	UFUNCTION(BlueprintCallable, Category = "Bind")
	FText BindPlayerName();

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
