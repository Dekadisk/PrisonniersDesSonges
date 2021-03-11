// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "SelectionWheelUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API USelectionWheelUserWidget : public UUserWidget
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintCallable, Category = "OnConstruct")
		void OnConstructOptions();
};
