// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/PlayerController.h"
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

	/* SPRAY */
	UPROPERTY()
	UMaterial* SelectionWheelMaterial;

	UPROPERTY()
	UUserWidget* SelectionWheel;

	TSubclassOf<UUserWidget> SelectionWheelWidgetClass;

public:
	
	virtual void BeginPlay() override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
