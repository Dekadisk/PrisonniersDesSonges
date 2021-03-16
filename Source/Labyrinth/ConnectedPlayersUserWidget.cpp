// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectedPlayersUserWidget.h"

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

void UConnectedPlayersUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UConnectedPlayersUserWidget, playersInfo);
}
