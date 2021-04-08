#pragma once

#include "Core.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInfo.h"
#include "Components/VerticalBox.h"
#include "Components/Button.h"
#include "ChatWindowUserWidget.h"
#include "LobbyMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API ULobbyMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnConstructLobby(); 

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void ClearPlayerList();

	//UFUNCTION(BlueprintCallable, Client, Reliable, Category = "Lobby")
	UFUNCTION(BlueprintImplementableEvent, Category = "Lobby")
	void UpdatePlayerWindow(FPlayerInfo playerInfo, int id);

	UFUNCTION(Category = "Lobby")
	void UpdatePlayersDisplay(int currentNumberPlayer);

	UFUNCTION(Category = "Lobby")
	void UpdateSeedDisplay(FText seed);

	UFUNCTION(Category = "Lobby")
	void UpdateStatus();

	//OnClick Function
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnClickLeaveLobby();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void OnClickReadyStart();

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickGameSettings();

	//Bind Function
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	FText BindServerName();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	FText BindPlayerDisplay();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	FText BindReadyButtonText();

	UFUNCTION(BlueprintCallable, Category = "Lobby")
	FText BindSeedDisplay();

	UFUNCTION(BlueprintCallable, Category = "Enable")
	bool EnableReadyButton();

	UFUNCTION(BlueprintCallable, Category = "TextChanged")
	void OnTextChangedSeed(FText seed);

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetLobbyCPP", meta = (BindWidget))
	UVerticalBox *PlayerWindow;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetLobbyCPP", meta = (BindWidget))
	UButton* SettingsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WidgetLobbyCPP", meta = (BindWidget))
	UChatWindowUserWidget* ChatWindow;

	UPROPERTY(Transient, Replicated)
	FName ServerName;

private:

	UPROPERTY(Transient, Replicated)
	FText PlayersDisplay;

	UPROPERTY()
	FText ReadyButtonText;

	UPROPERTY(Transient, Replicated)
	FText SeedDisplay;

	UPROPERTY()
	class ALobbyPlayerController *PlayerOwner;

};
