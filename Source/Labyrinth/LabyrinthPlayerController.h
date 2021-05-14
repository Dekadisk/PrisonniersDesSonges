#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "PlayerInfo.h"
#include "LabyrinthPlayerController.generated.h"

UCLASS()
class LABYRINTH_API ALabyrinthPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	ALabyrinthPlayerController();
	
public:

	// Inventaire
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Transient, Replicated)
	bool bHasKey;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Transient, Replicated)
	bool bHasLantern;

	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Transient, Replicated)
	bool bHasTrap;

	/* Used to know if the player currently has a chalk.*/
	UPROPERTY(EditDefaultsOnly, Category = "Inventory", Transient, Replicated)
	bool bHasChalk;

	UPROPERTY(EditDefaultsOnly, Category = "Status", Transient, Replicated)
	bool bIsHidden;

	UPROPERTY(EditDefaultsOnly, Category = "Status", Transient, Replicated)
	bool bIsInCupboard;

	UPROPERTY(BlueprintReadWrite, Category = "Status", Transient, Replicated)
	bool bIsDead;

	/* SPRAY */
	UPROPERTY()
	UMaterial* SelectionWheelMaterial;

	UPROPERTY()
	UUserWidget* SelectionWheel;

	UPROPERTY()
	UUserWidget* Scoreboard;

	TSubclassOf<UUserWidget> SelectionWheelWidgetClass;
	TSubclassOf<UUserWidget> IngameScoreboardWidgetClass;

	TSubclassOf<UUserWidget> ChatWidgetClass;
	TSubclassOf<UUserWidget> PauseWidgetClass;
	TSubclassOf<UUserWidget> DeathWidgetClass;

	UUserWidget* ChatWidget;
	UUserWidget* PauseWidget;
	UUserWidget* DeathWidget;

	UPROPERTY(Replicated)
	FText senderText;

	UPROPERTY(Replicated)
	FText senderName;

	UPROPERTY(Replicated, BlueprintReadWrite, EditAnywhere)
	AActor* pLantern;

	FString PlayerSettingsSaved;

	UPROPERTY(Replicated)
	FPlayerInfo playerSettings;

	UPROPERTY(Replicated)
	TArray<FText> playersNames;

	// Ordre : Lantern, Chalk, Key, Trap
	UPROPERTY(Replicated)
	TArray<bool> playersInventories1;

	UPROPERTY(Replicated)
		TArray<bool> playersInventories2;

	UPROPERTY(Replicated)
		TArray<bool> playersInventories3;

	UPROPERTY(Replicated)
		TArray<bool> playersInventories4;

	bool chatOn;

	bool pauseOn;

	bool gameEnded;

	FTimerHandle timerChatHandle;

	AActor* playerSpectating;

public:
	
	virtual void BeginPlay() override;

	virtual void SetupInputComponent() override;

	UFUNCTION(Reliable, Client)
	void SetupChatWindow();

	UFUNCTION(Reliable, Server)
	void ServerGetChatMsg(const FText& text);

	UFUNCTION(Reliable, Server)
	void ServerGetPlayersInfo();

	UFUNCTION(Reliable, Client)
	void UpdateChat(const FText& sender, const FText& text);

	UFUNCTION(Reliable, Server)
	void ServerGetPlayerInfo(FPlayerInfo playerSettingsInfo);

	UFUNCTION(Reliable, Client, Category = "PCLab")
	void Kicked();

	UFUNCTION(Reliable, Client)
	void PlayCutscene(int nbSurvivors);

	void ShowPauseMenu();

	UFUNCTION(Reliable, Client)
	void ShowDeathScreen();

	UFUNCTION(BlueprintCallable, Reliable, Client, Category = "PCLab")
	void Spectate();

	UFUNCTION()
	void ChangeSpectate();

	void LoadGame();

	void HideChat();

	void EndPlay(EEndPlayReason::Type reason) override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
