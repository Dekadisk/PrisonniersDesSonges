// Fill out your copyright notice in the Description page of Project Settings.


#include "OptionsMenuUserWidget.h"
#include "LabyrinthGameInstance.h"

void UOptionsMenuUserWidget::OnTextChangedUserPlayerName(FText name) {

	EnteredPlayerName = name;
	accept = !name.IsEmpty();
}

void UOptionsMenuUserWidget::OnClickBack() {
	
	RemoveFromParent();
	
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::OnClickAccept() {

	playerInfo.PlayerName = EnteredPlayerName.ToString();
	playerInfo.PlayerImage = Avatar;

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::OnClickToggleLeft() {

	nbAvatar = FMath::Clamp(nbAvatar - 1, 0, AvatarsList.Num() - 1);
	Avatar = AvatarsList[nbAvatar];
}

void UOptionsMenuUserWidget::OnClickToggleRight() {

	nbAvatar = FMath::Clamp(nbAvatar + 1, 0, AvatarsList.Num() - 1);
	Avatar = AvatarsList[nbAvatar];
}