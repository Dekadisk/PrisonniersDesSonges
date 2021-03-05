// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include <Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "ServerMenuUserWidget.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UServerMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OnConstruct")
	void OnConstructServer();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickBack();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickAccept();

	//UFUNCTION(BlueprintCallable, Category = "Session")
	void DisplaySession(FOnlineSessionSearchResult session);

	UFUNCTION(BlueprintCallable, Category = "ButtonInfo")
	bool GetAccept() { return accept; }

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickToggleRightServer();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
		void OnClickToggleLeftServer();

	UFUNCTION(BlueprintCallable, Category = "PlayMode")
		FText GetPlayModeH() { return PlayModeH; }

	UFUNCTION(BlueprintCallable, Category = "PlayMode")
		FText GetPlayMode() { return PlayMode; }

	UFUNCTION(BlueprintCallable, Category = "PlayMode")
		bool GetButtonVisibility() { return buttonVisible; }

	//UFUNCTION(BlueprintImplementableEvent, Category = "ServerMenu")
		//void SwitchButton();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Menu", meta = (BindWidget))
		UWidgetSwitcher* switchLoad;

private:

	UPROPERTY()
	FText PlayMode;
	bool lan;

	UPROPERTY()
	FText PlayModeH;

	bool sessionFound;
	FOnlineSessionSearchResult SessionAvailable;
	TArray<FOnlineSessionSearchResult> SessionsList;

	bool accept;
	bool buttonVisible;

	void RefreshServers();
	
};
