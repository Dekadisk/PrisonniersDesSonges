#pragma once

#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	FPlayerInfo();

	UPROPERTY(EditAnywhere)
		FString PlayerName;

	UPROPERTY(EditAnywhere)
		UTexture2D* PlayerImage;

	UPROPERTY(EditAnywhere)
		ACharacter* PlayerCharacter;

	UPROPERTY(EditAnywhere)
		FString PlayerStatus;
};