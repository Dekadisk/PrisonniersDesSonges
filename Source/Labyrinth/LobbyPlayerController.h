#pragma once

#include "Core.h"
#include "GameFramework/PlayerController.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInfo.h"
#include "LobbyMenuUserWidget.h"
#include "Blueprint/UserWidget.h"
#include "PlayerSaveGame.h"
#include "LobbyPlayerController.generated.h"

UCLASS()
class LABYRINTH_API ALobbyPlayerController : public APlayerController
{
	GENERATED_BODY()

public:

	ALobbyPlayerController();

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void InitialSetup();

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void AddPlayerInfo(const TArray<FPlayerInfo> &connectedPlayerInfo);

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void UpdateLocalSettings(int seed);

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void UpdateNumberPlayerDisplay(int currentNumberPlayer);

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void ShowLoadingScreen();

	UFUNCTION(Server, Reliable, WithValidation, Category = "PCLobby")
	void ServerCallUpdate(FPlayerInfo info);

	UFUNCTION(Server, Reliable, Category = "PCLobby")
	void ServerGetChatMsg(const FText& textToSend);

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void Kicked();

	UFUNCTION(Reliable, Client)
	void SetupLobbyMenu(const FName &ServerName);

	UFUNCTION(Reliable, Client)
	void UpdateChat(const FText& sender, const FText& text);

	UFUNCTION(Reliable, Client)
	void SaveSeed(int seed);

	UFUNCTION(Reliable, Client)
	void TravelToLvl();

	void SaveGameCheck();

	virtual void EndPlay(EEndPlayReason::Type reason) override;

public:

	UPROPERTY(Replicated)
	FPlayerInfo playerSettings;

	UPROPERTY(Replicated)
	FText senderText;

	UPROPERTY(Replicated)
	FText senderName;

	UPROPERTY(Replicated)
	bool IsServer = false;

private:

	void LoadGame();

	void SaveGame();

	UPlayerSaveGame* save;

	FString PlayerSettingsSaved;

	UPROPERTY(Transient, Replicated)
	TArray<FPlayerInfo> allConnectedPlayers;

	UPROPERTY()
	ULobbyMenuUserWidget* LobbyMenu;

	TOptional<int> partySeed;

	TSubclassOf<UUserWidget> LobbyMenuWidgetClass;

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
	
};
