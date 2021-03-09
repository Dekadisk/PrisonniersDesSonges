#include "LobbyMenuUserWidget.h"
#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>
#include "ConnectedPlayersUserWidget.h"

void ULobbyMenuUserWidget::OnConstructLobby()
{
	PlayerOwner = Cast<ALobbyPlayerController>(GetOwningPlayer());
	
	if(IsValid(PlayerOwner))
		if (GetWorld()->IsServer())
		{
			ReadyButtonText = FText::FromString("Start Session");
			PlayerOwner->playerSettings.PlayerStatus = true;
		}
		else
		{
			ReadyButtonText = FText::FromString("Toggle Ready");
			SettingsButton->RemoveFromParent();
		}

	//PlayerWindow = WidgetTree->ConstructWidget<UVerticalBox>(UVerticalBox::StaticClass(), FName("Player Window"));
}

void ULobbyMenuUserWidget::ClearPlayerList() {
	PlayerWindow->ClearChildren();
}

void ULobbyMenuUserWidget::UpdatePlayerWindow_Implementation(FPlayerInfo playerInfo)
{
	UConnectedPlayersUserWidget* connectedPlayers = WidgetTree->ConstructWidget<UConnectedPlayersUserWidget>(UConnectedPlayersUserWidget::StaticClass(), FName("Players Info"));
	connectedPlayers->playersInfo.Add(playerInfo);
	PlayerWindow->AddChild(connectedPlayers);
}

void ULobbyMenuUserWidget::UpdatePlayersDisplay(int currentNumberPlayer)
{
	PlayersDisplay = FText::FromString(FString::FromInt(currentNumberPlayer) + " sur 4");
}

void ULobbyMenuUserWidget::UpdateSeedDisplay(FText seed)
{
	if (FCString::Atoi(*seed.ToString()) != 0)
		SeedDisplay = seed;
	else
		SeedDisplay = FText::FromString("Random");
}

void ULobbyMenuUserWidget::UpdateStatus()
{
	PlayerOwner->playerSettings.PlayerStatus = !PlayerOwner->playerSettings.PlayerStatus;
	PlayerOwner->ServerCallUpdate(PlayerOwner->playerSettings);
}

void ULobbyMenuUserWidget::OnTextChangedSeed(FText seed)
{
	UpdateSeedDisplay(seed);
}

void ULobbyMenuUserWidget::OnClickLeaveLobby()
{
	if (IsValid(PlayerOwner)) {
		PlayerOwner->EndPlay(ServerName);
		UGameplayStatics::OpenLevel(GetWorld(), FName("Main"));
	}
}

void ULobbyMenuUserWidget::OnClickReadyStart()
{
	if (GetWorld()->IsServer())
	{
		ALobbyGameMode* lobbyGamemode = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode());
		if (IsValid(lobbyGamemode))
		{
			for (APlayerController* PC : lobbyGamemode->AllPlayerControllers)
			{
				ALobbyPlayerController* LobbyPC = Cast<ALobbyPlayerController>(PC);
				if (IsValid(LobbyPC))
				{
					LobbyPC->ShowLoadingScreen();
				}
			}
			lobbyGamemode->LaunchGame();
		}
	}
	else
	{
		UpdateStatus();
	}
}

void ULobbyMenuUserWidget::OnClickGameSettings()
{
	if (!GameSettings->IsVisible()) {
		GameSettings->SetVisibility(ESlateVisibility::Visible);
		Cast<UGameSettingsUserWidget>(GameSettings)->FillPlayersWindow();
	}
}

FText ULobbyMenuUserWidget::BindServerName()
{
	return FText::FromString(ServerName.ToString());
}

FText ULobbyMenuUserWidget::BindPlayerDisplay()
{
	return PlayersDisplay;
}

FText ULobbyMenuUserWidget::BindReadyButtonText()
{
	return ReadyButtonText;
}

FText ULobbyMenuUserWidget::BindSeedDisplay()
{
	return SeedDisplay;
}

bool ULobbyMenuUserWidget::EnableReadyButton()
{
	return Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->canStart;
}

void ULobbyMenuUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULobbyMenuUserWidget, ServerName);
	DOREPLIFETIME(ULobbyMenuUserWidget, PlayersDisplay);
	DOREPLIFETIME(ULobbyMenuUserWidget, SeedDisplay);
}
