// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UHostMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OnConstruct")
		void OnConstructHost();

	UFUNCTION(BlueprintCallable, Category = "OnTextChanged")
		void OnTextChangedServerName(FText name);

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickBackHost();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickAcceptHost();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickToggleRightHost();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickToggleLeftHost();

	UFUNCTION(BlueprintCallable, Category = "PlayMode")
		FText GetPlayMode() { return PlayMode; }

	UFUNCTION(BlueprintCallable, Category = "Accept")
		bool GetAccept() { return accept; }

private:

	FText ServerName;

	bool accept;

	bool lan;

	UPROPERTY()
	FText PlayMode;
};
