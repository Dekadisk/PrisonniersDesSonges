// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
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

	UFUNCTION()

};
