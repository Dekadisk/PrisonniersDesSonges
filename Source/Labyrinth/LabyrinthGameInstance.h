// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
//#include "Online.h"
//#include "UObject/UObjectGlobals.h"
#include "LabyrinthGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ULabyrinthGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	ULabyrinthGameInstance();

	//ULabyrinthGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION()
	void ShowMainMenu();

	UFUNCTION()
	void ShowHostMenu();

	UFUNCTION()
	void ShowServerMenu();

	UFUNCTION()
	void ShowOptionsMenu();

	UFUNCTION()
	void ShowLoadingScreen();

	UFUNCTION()
	void LaunchLobby(int nbPlayer, bool isLan, FText serverName);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:

	TSubclassOf<UUserWidget> MenuWidgetClass;
	TSubclassOf<UUserWidget> HostMenuWidgetClass;
	TSubclassOf<UUserWidget> ServerMenuWidgetClass;
	TSubclassOf<UUserWidget> OptionsMenuWidgetClass;

public:

	UPROPERTY()
	UUserWidget* MainMenu;

	UPROPERTY()
	UUserWidget* HostMenu;

	UPROPERTY()
	UUserWidget* ServerMenu;

	UPROPERTY()
	UUserWidget* OptionsMenu;

	UPROPERTY(EditDefaultsOnly, Category = "ServerSettings", Transient, Replicated)
	int MaxPlayers;

	UPROPERTY(EditDefaultsOnly, Category = "ServerSettings", Transient, Replicated)
	FText ServerName;

private:

	// Creer une session

	/*bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);*/

};
