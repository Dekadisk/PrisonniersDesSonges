// Fill out your copyright notice in the Description page of Project Settings.

#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "LabyrinthGameInstance.h"

ALobbyPlayerController::ALobbyPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyMenuWidget{ TEXT("/Game/UI/LobbyMenu") };
	LobbyMenuWidgetClass = LobbyMenuWidget.Class;
	ready = false;
	bReplicates = true;
}

void ALobbyPlayerController::InitialSetup_Implementation()
{
	SaveGameCheck();
	ServerCallUpdate(playerSettings, false);
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

void ALobbyPlayerController::ServerCallUpdate_Implementation(FPlayerInfo info, bool isReady)
{
	ALobbyGameMode* lobbyGamemode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
	if (IsValid(lobbyGamemode))
	{
		playerSettings = info;
		ready = isReady;

		lobbyGamemode->ServerEveryoneUpdate();
	}
	
}

bool ALobbyPlayerController::ServerCallUpdate_Validate(FPlayerInfo info, bool isReady)
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

void ALobbyPlayerController::EndPlay()
{
	ULabyrinthGameInstance* lobbyGameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());
	if (IsValid(lobbyGameInst))
	{
		// Je dois détruire la session mais je sias pas comment faire
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
