// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Party.generated.h"


UCLASS(BlueprintType)
class LABYRINTH_API UParty : public UObject
{
	GENERATED_BODY()

public:
	UParty() = default;
	~UParty() = default;
public:
	UPROPERTY(BlueprintReadOnly)
	FString partyId;

	UPROPERTY(BlueprintReadOnly)
	FString serverName;

	UPROPERTY(BlueprintReadOnly)
	int maxPlayers;

	UPROPERTY(BlueprintReadOnly)
	int nbSurvivor;

	UPROPERTY(BlueprintReadOnly)
	int seedUsed;

	UPROPERTY(BlueprintReadOnly)
	int64 partyDuration;

	UPROPERTY(BlueprintReadOnly)
	TArray<FString> listPlayerName;

	UPROPERTY(BlueprintReadOnly)
	int leaderBoardPosition;
};
