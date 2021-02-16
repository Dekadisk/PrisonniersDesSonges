// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "MainMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickHost();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickFind();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickOptions();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickQuit();


	// TEMPORAIRE
	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void tempOnClickPlay();
	
};
