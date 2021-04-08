// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSettingsUserWidget.h"
#include "LobbyGameMode.h"

//void UGameSettingsUserWidget::FillPlayersWindow() {
//	//Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())
//}

void UGameSettingsUserWidget::OnClickAccept() {
	SetVisibility(ESlateVisibility::Hidden);
}

void UGameSettingsUserWidget::OnClickBack() {
	SetVisibility(ESlateVisibility::Hidden);
}