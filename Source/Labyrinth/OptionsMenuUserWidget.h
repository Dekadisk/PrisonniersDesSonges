// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "OptionsMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UOptionsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UOptionsMenuUserWidget();

private:

	UPROPERTY()
	uint8 MsgVisibility;

	UPROPERTY()
	FString EnteredTypeName;

	UPROPERTY()
	FPlayerInfo playerInfo;
	
};
