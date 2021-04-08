// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectedPlayersUserWidget.h"
#include "LobbyGameMode.h"
#include "LobbyPlayerController.h"

void UConnectedPlayersUserWidget::OnClickKick() {
	Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->ServerGetKicked(id);
	RemoveFromParent();
}

FText UConnectedPlayersUserWidget::BindStatus()
{
	if (playersInfo.PlayerStatus) {
		return FText::FromString("Ready !");
	}
	else {
		return FText::FromString("Not Ready !");
	}
}

FText UConnectedPlayersUserWidget::BindPlayerName()
{
	return playersInfo.PlayerName;
}

bool UConnectedPlayersUserWidget::BindKickVisibility() {
	return GetWorld()->IsServer() && Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->ServerPlayerName.ToString() != playersInfo.PlayerName.ToString();
}

void UConnectedPlayersUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UConnectedPlayersUserWidget, playersInfo);
}
