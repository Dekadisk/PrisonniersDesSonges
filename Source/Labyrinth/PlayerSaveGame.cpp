// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerSaveGame.h"
#include <Runtime/Engine/Public/Net/UnrealNetwork.h>

UPlayerSaveGame::UPlayerSaveGame() {
	pInfo.PlayerCharacter = nullptr;
	pInfo.PlayerName = FText::FromString("Player");
}

void UPlayerSaveGame::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerSaveGame, pInfo);
}