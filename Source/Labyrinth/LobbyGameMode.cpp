// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include <Net/UnrealNetwork.h>

void ALobbyGameMode::ServerEveryoneUpdate_Implementation() {

}

bool ALobbyGameMode::ServerEveryoneUpdate_Validate() {
	return true;
}

void ALobbyGameMode::LaunchGame()
{
}

void ALobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameMode, AllPlayerController);
}
