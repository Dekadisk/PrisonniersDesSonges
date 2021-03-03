// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenuUserWidget.h"
#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"

void ULobbyMenuUserWidget::OnConstructLobby()
{
	PlayerOwner = Cast<ALobbyPlayerController>(GetOwningPlayer());
	if(IsValid(PlayerOwner))
		if (isServer)
		{
			ReadyButtonText = FText::FromString("Start Session");
			PlayerOwner->playerSettings.PlayerStatus = true;
		}
		else
		{
			ReadyButtonText = FText::FromString("Toggle Ready");
		}
}

void ULobbyMenuUserWidget::ClearPlayerList() {

}

void ULobbyMenuUserWidget::UpdatePlayerWindow_Implementation(FPlayerInfo playerInfo)
{
}

void ULobbyMenuUserWidget::UpdatePlayersDisplay(int currentNumberPlayer)
{
	PlayersDisplay = FText::FromString(FString::FromInt(currentNumberPlayer) + " sur 4");
}

void ULobbyMenuUserWidget::UpdateStatus()
{
	PlayerOwner->playerSettings.PlayerStatus = !PlayerOwner->playerSettings.PlayerStatus;
	PlayerOwner->ServerCallUpdate(PlayerOwner->playerSettings);
}

void ULobbyMenuUserWidget::OnClickLeaveLobby()
{
	if (IsValid(PlayerOwner))
		PlayerOwner->EndPlay(FName(ServerName.ToString()));
}

void ULobbyMenuUserWidget::OnClickReadyStart()
{
	if (isServer)
	{
		ALobbyGameMode* lobbyGamemode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(lobbyGamemode))
		{
			for (APlayerController* PC : lobbyGamemode->AllPlayerController)
			{
				ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
				if (IsValid(LobbyPC))
				{
					LobbyPC->ShowLoadingScreen();
				}
			}
			lobbyGamemode->LaunchGame();
		}

	}
	else
	{
		UpdateStatus();
		//TO DO 
	}
}

void ULobbyMenuUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULobbyMenuUserWidget, ServerName);
	DOREPLIFETIME(ULobbyMenuUserWidget, PlayersDisplay);
}