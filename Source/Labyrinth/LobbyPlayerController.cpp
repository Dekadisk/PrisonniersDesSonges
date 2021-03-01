// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"

ALobbyPlayerController::ALobbyPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyMenuWidget{ TEXT("/Game/UI/LobbyMenu") };
	LobbyMenuWidgetClass = LobbyMenuWidget.Class;

	bReplicates = true;
}

void ALobbyPlayerController::OnRep_InitialSetup()
{
	SaveGameCheck();
	ServerCallUpdate(playerSettings, false);
}

void ALobbyPlayerController::ServerCallUpdate(FPlayerInfo info, bool ChangedStatus)
{
	playerSettings = info;
}

void ALobbyPlayerController::OnRep_SetupLobbyMenu(FText ServerName) {
	SetShowMouseCursor(true);

	UUserWidget* LobbyMenu = CreateWidget<UUserWidget>(this, LobbyMenuWidgetClass);
	LobbyMenu->AddToViewport();

	FInputModeGameAndUI mode;
	mode.SetHideCursorDuringCapture(true);
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	mode.SetWidgetToFocus(LobbyMenu->TakeWidget());

	SetInputMode(mode);
}

void ALobbyPlayerController::SaveGameCheck()
{
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
