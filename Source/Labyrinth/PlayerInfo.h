#pragma once

#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FText PlayerName;

	UPROPERTY(EditAnywhere)
	ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
	bool PlayerStatus = false;

	UPROPERTY(EditAnywhere)
	FText TextureQuality = FText::FromString("1");

	UPROPERTY(EditAnywhere)
	FText ShadowQuality = FText::FromString("1");

	UPROPERTY(EditAnywhere)
	FText PostQuality = FText::FromString("1");

	UPROPERTY(EditAnywhere)
	FText Language = FText::FromString("Francais");

	UPROPERTY(EditAnywhere)
	FText Resolution = FText::FromString("1920x1080");

	UPROPERTY(EditAnywhere)
	bool Fullscreen = true;

	UPROPERTY(EditAnywhere)
	float MasterVolume = 1.f;

	UPROPERTY(EditAnywhere)
	float MusicVolume = 1.f;

	UPROPERTY(EditAnywhere)
	float SFXVolume = 1.f;

	/* BACK END INFO */
	UPROPERTY(EditAnywhere)
	FString UserId;

	UPROPERTY(EditAnywhere)
	FString GuestToken;

	UPROPERTY(EditAnywhere)
	FString SessionToken;
};