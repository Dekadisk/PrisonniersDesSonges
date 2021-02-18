// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UServerMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OnTextChanged")
	void OnTextChangedIpAddress(FText ip);

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickBack();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickAccept();

	//UFUNCTION(BlueprintCallable, Category = "Session")
	//void DisplaySession();

	UFUNCTION(BlueprintCallable, Category = "ButtonInfo")
	bool GetAccept() { return accept; }

private:

	UPROPERTY()
	FText EnteredIpAddress;

	bool accept;
	
};
