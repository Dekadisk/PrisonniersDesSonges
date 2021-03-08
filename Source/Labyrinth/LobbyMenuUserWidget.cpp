#include "LobbyMenuUserWidget.h"
#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "Components/TextBlock.h"
#include "Blueprint/WidgetTree.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

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
	UTextBlock *text = WidgetTree->ConstructWidget<UTextBlock>(UTextBlock::StaticClass(), FName("Players Info"));
	text->Text = FText::FromString(playerInfo.PlayerName.ToString() + " " + (playerInfo.PlayerStatus ? "Ready" : "Not Ready"));
	PlayerWindow->AddChild(text);
}

void ULobbyMenuUserWidget::UpdatePlayersDisplay(int currentNumberPlayer)
{
	PlayersDisplay = FText::FromString(FString::FromInt(currentNumberPlayer) + " sur 4");
}

void ULobbyMenuUserWidget::UpdateSeedDisplay(int seed)
{
	if (seed != 0)
		SeedDisplay = FText::AsNumber(seed);
	else
		SeedDisplay = FText::FromString("Random");
}

void ULobbyMenuUserWidget::UpdateStatus()
{
	PlayerOwner->playerSettings.PlayerStatus = !PlayerOwner->playerSettings.PlayerStatus;
	PlayerOwner->ServerCallUpdate(PlayerOwner->playerSettings);
}

void ULobbyMenuUserWidget::OnClickLeaveLobby()
{
	if (IsValid(PlayerOwner)) {
		PlayerOwner->EndPlay(FName(ServerName.ToString()));
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

FText ULobbyMenuUserWidget::BindServerName()
{
	return ServerName;
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

void ULobbyMenuUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULobbyMenuUserWidget, ServerName);
	DOREPLIFETIME(ULobbyMenuUserWidget, PlayersDisplay);
	DOREPLIFETIME(ULobbyMenuUserWidget, SeedDisplay);
}
