// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "OptionsMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UOptionsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OnConstruct")
		void OnConstructOptions();

	UFUNCTION(BlueprintCallable, Category = "OnTextChanged")
		void OnTextChangedUserPlayerName(FText name);

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickBackOptions();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickAcceptOptions();

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		FText GetEnteredPlayerName() { return EnteredPlayerName; }

	UFUNCTION(BlueprintCallable, Category = "Accept")
		bool GetAccept() { return accept; }

	UFUNCTION(BlueprintCallable, Category = "Back")
		bool BackVisible() { return saveFound; }

private:

	UPROPERTY()
	FPlayerInfo playerInfo;

	UPROPERTY()
	FText EnteredPlayerName;

	UPROPERTY()
	bool accept;

	UPROPERTY()
	FString PlayerSettingsSaved;

	bool saveFound;

	void SaveGameCheck();

	void EmptyNameCheck(FText name);

	void SaveGame();

	void LoadGame();
	
};
