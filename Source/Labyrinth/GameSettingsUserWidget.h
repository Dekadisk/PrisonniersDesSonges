// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSettingsUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UGameSettingsUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintImplementableEvent, Category = "GameSettingsMenu")
	void FillPlayersWindow();

	UFUNCTION(BlueprintCallable, Category = "GameSettingsMenu")
	void OnClickAccept();

	UFUNCTION(BlueprintCallable, Category = "GameSettingsMenu")
	void OnClickBack();
};