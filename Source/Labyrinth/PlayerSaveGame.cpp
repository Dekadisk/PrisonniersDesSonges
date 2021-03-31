#include "PlayerSaveGame.h"
#include "Net/UnrealNetwork.h"

UPlayerSaveGame::UPlayerSaveGame() {
	pInfo.PlayerCharacter = nullptr;
	pInfo.PlayerName = FText::FromString("Player");
}

void UPlayerSaveGame::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {

	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UPlayerSaveGame, pInfo);
}