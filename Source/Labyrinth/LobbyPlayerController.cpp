// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "LabyrinthGameInstance.h"

ALobbyPlayerController::ALobbyPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyMenuWidget{ TEXT("/Game/UI/LobbyMenu") };
	LobbyMenuWidgetClass = LobbyMenuWidget.Class;
	bReplicates = true;
}

void ALobbyPlayerController::InitialSetup_Implementation()
{
	SaveGameCheck();
	ServerCallUpdate(playerSettings);
}

void ALobbyPlayerController::AddPlayerInfo_Implementation(const TArray<FPlayerInfo> &connectedPlayerInfo)
{
	allConnectedPlayers = connectedPlayerInfo;
	if (IsValid(LobbyMenu))
	{
		LobbyMenu->ClearPlayerList();
		for (FPlayerInfo playerInfo : connectedPlayerInfo)
			LobbyMenu->UpdatePlayerWindow(playerInfo);
	}
	
	
}

void ALobbyPlayerController::UpdateLocalSettings_Implementation(int seed)
{
	if (seed != 0)
	{
		partySeed.Emplace(seed);
	}
	else
	{
		partySeed.Reset();
	}
	LobbyMenu->UpdateSeedDisplay(seed);
}

void ALobbyPlayerController::UpdateNumberPlayerDisplay_Implementation(int currentNumberPlayer)
{
	if(IsValid(LobbyMenu))
		LobbyMenu->UpdatePlayersDisplay(currentNumberPlayer);
}

void ALobbyPlayerController::ShowLoadingScreen_Implementation()
{
	if (IsValid(LobbyMenu))
	{
		ULabyrinthGameInstance* lobbyGameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());
		if (IsValid(lobbyGameInst))
		{
			LobbyMenu->RemoveFromParent();
			lobbyGameInst->ShowLoadingScreen();
		}
	}
}

void ALobbyPlayerController::ServerCallUpdate_Implementation(FPlayerInfo info)
{
	ALobbyGameMode* lobbyGamemode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(lobbyGamemode))
	{
		playerSettings = info;

		lobbyGamemode->ServerEveryoneUpdate();
	}
	
}

bool ALobbyPlayerController::ServerCallUpdate_Validate(FPlayerInfo info)
{
	return true;
}

void ALobbyPlayerController::SetupLobbyMenu_Implementation(const FText &ServerName) {

	SetShowMouseCursor(true);

	LobbyMenu = CreateWidget<ULobbyMenuUserWidget>(this, LobbyMenuWidgetClass);
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

bool ALobbyPlayerController::EndPlay(FName SessionName)
{
	
	if (IsLocalController())
	{
		ULabyrinthGameInstance* lobbyGameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());
		if (IsValid(lobbyGameInst))
		{
			return lobbyGameInst->DestroySession(SessionName);
		}
	}
	return false;
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
