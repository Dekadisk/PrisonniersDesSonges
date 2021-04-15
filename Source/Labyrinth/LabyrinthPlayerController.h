// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
#include "PlayerInfo.h"
#include "LabyrinthPlayerController.generated.h"

/**
 * 
 */
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

	/* SPRAY */
	UPROPERTY()
	UMaterial* SelectionWheelMaterial;

	UPROPERTY()
	UUserWidget* SelectionWheel;

	TSubclassOf<UUserWidget> SelectionWheelWidgetClass;

	TSubclassOf<UUserWidget> ChatWidgetClass;
	TSubclassOf<UUserWidget> PauseWidgetClass;

	UUserWidget* ChatWidget;
	UUserWidget* PauseWidget;

	UPROPERTY(Replicated)
	FText senderText;

	UPROPERTY(Replicated)
	FText senderName;

	FString PlayerSettingsSaved;

	UPROPERTY(Replicated)
	FPlayerInfo playerSettings;

	bool chatOn;

	bool pauseOn;

public:
	
	virtual void BeginPlay() override;

	UFUNCTION(Reliable, Client)
	void SetupChatWindow();

	UFUNCTION(Reliable, Server)
	void ServerGetChatMsg(const FText& text);

	UFUNCTION(Reliable, Client)
	void UpdateChat(const FText& sender, const FText& text);

	UFUNCTION(Reliable, Server)
	void ServerGetPlayerInfo(FPlayerInfo playerSettingsInfo);

	UFUNCTION(Reliable, Client, Category = "PCLobby")
	void Kicked();

	void ShowPauseMenu();

	void LoadGame();
	virtual void Tick(float deltaSeconds);

	void EndPlay(EEndPlayReason::Type reason) override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
