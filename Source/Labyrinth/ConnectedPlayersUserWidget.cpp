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
		if (playersInfo.Language.ToString() == "English")
			return FText::FromString("Ready !");
		else
			return FText::FromString("Pret !");
	}
	else {
		if (playersInfo.Language.ToString() == "English")
			return FText::FromString("Not ready !");
		else
			return FText::FromString("Se prepare !");
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
