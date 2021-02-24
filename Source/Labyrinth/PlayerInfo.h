#pragma once

#include "PlayerInfo.generated.h"

USTRUCT(BlueprintType)
struct FPlayerInfo
{
	GENERATED_BODY()

	FPlayerInfo() = default;

	UPROPERTY(EditAnywhere)
		FText PlayerName;

	UPROPERTY(EditAnywhere)
		ACharacter* PlayerCharacter;
};