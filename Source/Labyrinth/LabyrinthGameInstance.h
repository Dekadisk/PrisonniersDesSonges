// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include "PlayerSaveGame.h"
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

	ULabyrinthGameInstance() = default;

	ULabyrinthGameInstance(const FObjectInitializer& ObjectInitializer);

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
	void LaunchLobby(int32 nbPlayers, bool lan, FText ServerName);

	//UFUNCTION()
	void JoinServer(FName SessionName, FOnlineSessionSearchResult SessionToJoin);

	bool DestroySession(FName SessionName);

	UFUNCTION()
	void SaveGameCheck();

	void SetFileSaved(bool saved) { fileSaved = saved; }
	bool GetFileSaved() { return fileSaved; }

	FString GetFileName() { return SaveName; }
	UPlayerSaveGame* GetSaveFile() { return save; }

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	TSharedPtr<class FOnlineSessionSearch> GetSessionSearch() { return SessionSearch; }

private:

	TSubclassOf<UUserWidget> MenuWidgetClass;
	TSubclassOf<UUserWidget> HostMenuWidgetClass;
	TSubclassOf<UUserWidget> ServerMenuWidgetClass;
	TSubclassOf<UUserWidget> OptionsMenuWidgetClass;
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;

	int32 maxPlayers;
	FText ServerName;

	FString SaveName;

	UPROPERTY()
	UPlayerSaveGame* save;

	bool fileSaved;

public:

	UPROPERTY()
	UUserWidget* MainMenu;

	UPROPERTY()
	UUserWidget* HostMenu;

	UPROPERTY()
	UUserWidget* ServerMenu;

	UPROPERTY()
	UUserWidget* OptionsMenu;

	UPROPERTY()
	UUserWidget* LoadingScreen;

private:

	// Creer une session

	bool HostSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, bool bIsLan, bool bIsPresence, int32 MaxNumPlayers, FText MapName);

	FOnCreateSessionCompleteDelegate OnCreateSessionCompleteDelegate;
	FOnStartSessionCompleteDelegate OnStartSessionCompleteDelegate;

	FDelegateHandle OnCreateSessionCompleteDelegateHandle;
	FDelegateHandle OnStartSessionCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSettings> SessionSettings;

	void OnCreateSessionComplete(FName SessionName, bool bWasSuccessful);

	void OnStartOnlineGameComplete(FName SessionName, bool bWasSuccessful);
	
	// Trouver une session

	FOnFindSessionsCompleteDelegate OnFindSessionsCompleteDelegate;
	FDelegateHandle OnFindSessionsCompleteDelegateHandle;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void OnFindSessionsComplete(bool bWasSuccessful);

	// Rejoindre une session

	bool JoinSession(TSharedPtr<const FUniqueNetId> UserId, FName SessionName, const FOnlineSessionSearchResult& SearchResult);

	FOnJoinSessionCompleteDelegate OnJoinSessionCompleteDelegate;
	FDelegateHandle OnJoinSessionCompleteDelegateHandle;

	void OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result);

	// Detruire une session

	FOnDestroySessionCompleteDelegate OnDestroySessionCompleteDelegate;
	FDelegateHandle OnDestroySessionCompleteDelegateHandle;

	virtual void OnDestroySessionComplete(FName SessionName, bool bWasSuccessful);

};