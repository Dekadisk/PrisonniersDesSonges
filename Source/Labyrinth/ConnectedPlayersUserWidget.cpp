// Fill out your copyright notice in the Description page of Project Settings.


#include "ConnectedPlayersUserWidget.h"

void UConnectedPlayersUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(UConnectedPlayersUserWidget, playersInfo);
}
