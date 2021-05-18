#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "Engine/GameInstance.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include "PlayerSaveGame.h"
#include "Runtime/Online/HTTP/Public/Http.h"
#include "Party.h"
#include "Misc/ScopeLock.h"
#include "LabyrinthGameInstance.generated.h"


UCLASS()
class LABYRINTH_API ULabyrinthGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:

	ULabyrinthGameInstance(const FObjectInitializer& ObjectInitializer);

public:

	/* MAIN MENU */
	void ShowMainMenu();

	void ShowHostMenu();

	void ShowServerMenu();

	void ShowLeaderBoardMenu();

	void ShowOptionsMenu();

	void ShowNameMenu();

	void ShowLoadingScreen();

	void ShowLoadingScreen(APlayerController* playerController);

	void ShowTitleScreen();

	/* SESSION */
	void LaunchLobby(int32 nbPlayers, bool lan, FName ServerName);

	void FindSessions(TSharedPtr<const FUniqueNetId> UserId, bool bIsLAN, bool bIsPresence);

	TSharedPtr<class FOnlineSessionSearch> GetSessionSearch() { return SessionSearch; }
	void ClearSessionSearch() { SessionSearch.Reset(); }

	void JoinServer(FName SessionName, FOnlineSessionSearchResult SessionToJoin);

	bool DestroySession(FName SessionName);

	/* SAVE */
	void SaveGameCheck();

	UFUNCTION(BlueprintCallable)
	void OnClickTitleScreen();

	void ExecOptions();

	void SetFileSaved(bool saved) { fileSaved = saved; }
	bool GetFileSaved() { return fileSaved; }

	FString GetFileName() { return SaveName; }
	UPlayerSaveGame* GetSaveFile() { return save; }


	/* BACK END */

	UFUNCTION(BlueprintCallable)
	void LoginOnScoreServer();

	UFUNCTION(BlueprintCallable)
	void CreateUserOnScoreServer();

	UFUNCTION(BlueprintCallable)
	void CreateSessionOnScoreServer();

	UFUNCTION(BlueprintCallable)
	void RefreshSessionOnScoreServer();

	UFUNCTION(BlueprintCallable)
	void ChangeDBNameOnScoreServer(FString newName);

	UFUNCTION(BlueprintCallable)
	bool IsOfflineMod();

	UFUNCTION(BlueprintCallable)
	void ResetWaitingInfo();

	void SetStartTime();

	void CaculatePartyDuration();


	UFUNCTION(BlueprintCallable)
	void CreatePartyDB();

	UFUNCTION(BlueprintCallable)
	void GetBestPartyOfPlayer();

	UFUNCTION(BlueprintCallable)
	void AddPlayerToParty(FString partyId);

	UFUNCTION(BlueprintCallable)
	void GetTop10Party();

private:

	TSubclassOf<UUserWidget> MenuWidgetClass;
	TSubclassOf<UUserWidget> HostMenuWidgetClass;
	TSubclassOf<UUserWidget> ServerMenuWidgetClass;
	TSubclassOf<UUserWidget> LeaderBoardWidgetClass;
	TSubclassOf<UUserWidget> OptionsMenuWidgetClass;
	TSubclassOf<UUserWidget> NameMenuWidgetClass;
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass;
	TSubclassOf<UUserWidget> TitleScreenWidgetClass;

	FString SaveName;

	UPROPERTY()
	UPlayerSaveGame* save;

	bool fileSaved;

	/* BACK END */
	UPROPERTY()
	FString API_ENDPOINT = "http://10.38.133.111:5000/api/";

	UPROPERTY()
	bool offlineMod = false;

public:
	UPROPERTY()
	UParty* currentPartyDataForSave;

	UPROPERTY(BlueprintReadOnly)
	UParty* bestGameResult;

	UPROPERTY(BlueprintReadOnly)
	TArray<UParty*> top10Result;

private:
	UPROPERTY()
	bool waitingMoreInfo = true;

public:
	/* MAIN MENU */
	UPROPERTY()
	UUserWidget* MainMenu;
	UPROPERTY()
	UUserWidget* HostMenu;
	UPROPERTY()
	UUserWidget* ServerMenu;
	UPROPERTY()
	UUserWidget* LeaderBoardMenu;
	UPROPERTY()
	UUserWidget* OptionsMenu;
	UPROPERTY()
	UUserWidget* NameMenu;
	UPROPERTY()
	UUserWidget* LoadingScreen;

	UPROPERTY(BlueprintReadWrite)
	UUserWidget* TitleScreen;

	FName SessionName;

	//int seed = 0;

	UPROPERTY(BlueprintReadWrite)
	bool onTitle;

	bool FromLobby = false;

private:
	/* SESSION */

	FString StartingLevel = "Lobby";

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

	/* BACK END */
	void OnCreateUserCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void OnCreateSessionCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void OnRefreshSessionCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void OnChangeDBNameCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);

	void OnCreatePartyCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void OnGetBestPartyOfPlayerCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void OnAddPlayerToPartyCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
	void OnGetTop10PartyCompleted(FHttpRequestPtr request, FHttpResponsePtr response, bool bWasSuccessful);
};
