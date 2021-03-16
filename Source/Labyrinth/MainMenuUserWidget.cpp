// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuUserWidget.h"
#include "LabyrinthGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

void UMainMenuUserWidget::OnConstructMain() {
	FInputModeUIOnly mode;
	mode.SetWidgetToFocus(TakeWidget());
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(mode);
}

void UMainMenuUserWidget::OnClickHost() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowHostMenu();
	//instance->LaunchLobby();
}

void UMainMenuUserWidget::OnClickFind() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowServerMenu();
}

void UMainMenuUserWidget::OnClickOptions() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowOptionsMenu();
}

void UMainMenuUserWidget::OnClickQuit() {

	UKismetSystemLibrary::QuitGame(GetWorld(), GetOwningPlayer(), EQuitPreference::Quit, false);
}

// TEMPORAIRE

void UMainMenuUserWidget::tempOnClickPlay() {
	
	UGameplayStatics::OpenLevel(GetWorld(), "firstMap");

	RemoveFromParent();
}