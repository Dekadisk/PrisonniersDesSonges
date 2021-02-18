// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "OptionsMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UOptionsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OnTextChanged")
		void OnTextChangedUserPlayerName(FText name);

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickBack();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickAccept();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickToggleRight();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickToggleLeft();

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		FText GetEnteredPlayerName() { return EnteredPlayerName; }

	UFUNCTION(BlueprintCallable, Category = "PlayerInfo")
		UTexture2D* GetAvatar() { return Avatar; }

	UFUNCTION(BlueprintCallable, Category = "ButtonInfo")
		bool GetAccept() { return accept; }
	
	UPROPERTY(EditDefaultsOnly, Category = "Avatars")
		TArray<UTexture2D*> AvatarsList;

private:

	UPROPERTY()
	FPlayerInfo playerInfo;

	UPROPERTY()
	UTexture2D* Avatar;

	UPROPERTY()
	FText EnteredPlayerName;

	int nbAvatar;

	UPROPERTY()
	bool accept;
	
};
