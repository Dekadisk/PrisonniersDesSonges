// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyMenuUserWidget.h"

void ULobbyMenuUserWidget::ClearPlayerList() {

}

void ULobbyMenuUserWidget::OnRep_UpdatePlayerWindow()
{
}

void ULobbyMenuUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULobbyMenuUserWidget, infos);
}