// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenuUserWidget.h"
#include "LabyrinthGameInstance.h"

void UOptionsMenuUserWidget::OnConstructOptions() {

	EnteredPlayerName = playerInfo.PlayerName;
}

void UOptionsMenuUserWidget::OnTextChangedUserPlayerName(FText name) {

	EnteredPlayerName = name;
	accept = !name.IsEmpty();
}

void UOptionsMenuUserWidget::OnClickBackOptions() {
	
	RemoveFromParent();
	
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::OnClickAcceptOptions() {

	playerInfo.PlayerName = EnteredPlayerName;

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::OnClickToggleLeftOptions() {

	nbAvatar = FMath::Clamp(nbAvatar - 1, 0, AvatarsList.Num() - 1);
	Avatar = AvatarsList[nbAvatar];
}

void UOptionsMenuUserWidget::OnClickToggleRightOptions() {

	nbAvatar = FMath::Clamp(nbAvatar + 1, 0, AvatarsList.Num() - 1);
	Avatar = AvatarsList[nbAvatar];
}