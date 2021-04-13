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
	FText Language = FText::FromString("English");

	UPROPERTY(EditAnywhere)
	FText Resolution = FText::FromString("800x600");
};