// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BackEndScoreBoardWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UBackEndScoreBoardWidget : public UUserWidget
{
	GENERATED_BODY()

	/*UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnClickHost();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnClickFind();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnclickLeaderBoard();*/

	UFUNCTION(BlueprintCallable, Category = "ScoreBoard")
	void OnClickBack();
};
