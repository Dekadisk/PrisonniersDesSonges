// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "IngameScoreboard.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UIngameScoreboard : public UUserWidget
{
	GENERATED_BODY()
public:
	UIngameScoreboard(const FObjectInitializer& ObjectInitializer);
public:

	TArray<FText> playersNames;

	// Ordre : Lantern, Chalk, Key, Trap
	TArray<TArray<bool>> playersInventories;

	AActor* owner;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureBackground;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureChalk;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureTrap;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureLantern;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
		UTexture2D* textureKey;

	UFUNCTION(Server, Reliable, WithValidation)
	void GetPlayersInfo();

	UFUNCTION(BlueprintCallable)
	int GetNumberOfItems(int numPlayer);

	UFUNCTION(BlueprintCallable)
	int GetNumberOfPlayers();
	
	UFUNCTION(BlueprintCallable)
	FText GetPlayerName(int playerNumber);

	UFUNCTION()
	TArray<bool> GetPlayerInventory(int playerNumber);

	UFUNCTION()
	uint32 GetItemType(int playerNumber, int itemNumber);

	UFUNCTION(BlueprintCallable)
		FSlateBrush GetImage(int playerNumber, int itemNumber);
};
