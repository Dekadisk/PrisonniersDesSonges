#include "LobbyMenuUserWidget.h"
#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "LabyrinthGameInstance.h"
#include "Kismet/GameplayStatics.h"

void ULobbyMenuUserWidget::OnConstructLobby()
{
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut Lobby");
	PlayerOwner = Cast<ALobbyPlayerController>(GetOwningPlayer());
	Cast<ULabyrinthGameInstance>(GetGameInstance())->FromLobby = false;
	
	if (IsValid(PlayerOwner)) {

		if (GetWorld()->IsServer())
		{
			if (PlayerOwner->playerSettings.Language.ToString() == "English")
				ReadyButtonText = FText::FromString("Start Session");
			else
				ReadyButtonText = FText::FromString("Debuter la partie");

			PlayerOwner->playerSettings.PlayerStatus = true;
		}
		else
		{
			if (PlayerOwner->playerSettings.Language.ToString() == "English")
				ReadyButtonText = FText::FromString("Toggle ready");
			else
				ReadyButtonText = FText::FromString("Pret ?");
		}
	}

	FInputModeUIOnly mode;
	mode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	mode.SetWidgetToFocus(TakeWidget());
	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetInputMode(mode);
}

void ULobbyMenuUserWidget::ClearPlayerList() {
	PlayerWindow->ClearChildren();
}

void ULobbyMenuUserWidget::UpdatePlayersDisplay(int currentNumberPlayer)
{
	PlayersDisplay = FText::FromString(FString::FromInt(currentNumberPlayer) + " sur 4");
}

void ULobbyMenuUserWidget::UpdateSeedDisplay(FText seed)
{
	if (FCString::Atoi(*seed.ToString()) != 0)
		SeedDisplay = seed;
	else if (PlayerOwner->playerSettings.Language.ToString() == "Francais")
		SeedDisplay = FText::FromString("Aleatoire");
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
	ULabyrinthGameInstance* inst = Cast<ULabyrinthGameInstance>(GetGameInstance());
	inst->FromLobby = true;
	if (!GetWorld()->IsServer()) {
		if (IsValid(PlayerOwner)) {
			PlayerOwner->EndPlay(EEndPlayReason::LevelTransition);
			UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
		}
	}
	else {
		TArray<APlayerController*> pcs = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->AllPlayerControllers;
		for (APlayerController* pc : pcs) {
			if (pc->GetNetMode() == ENetMode::NM_Client)
				Cast<ALobbyPlayerController>(pc)->Kicked();
		}

		if (IsValid(PlayerOwner)) {
			PlayerOwner->EndPlay(EEndPlayReason::LevelTransition);
			UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
		}
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
					LobbyPC->ShowLoadingScreen();
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
	/*if (!GameSettingsMenu->IsVisible()) {
		GameSettingsMenu->SetVisibility(ESlateVisibility::Visible);
		GameSettingsMenu->FillPlayersWindow();
	}*/
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
	if (GetOwningPlayer()->HasAuthority())
		return Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->canStart;
	else
		return true;
}


void ULobbyMenuUserWidget::OnTextChangedSeed(FText seed)
{
	UpdateSeedDisplay(seed);
	Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->ServerUpdateGameSettings(FCString::Atoi(*seed.ToString()));
}

void ULobbyMenuUserWidget::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ULobbyMenuUserWidget, ServerName);
	DOREPLIFETIME(ULobbyMenuUserWidget, PlayersDisplay);
	DOREPLIFETIME(ULobbyMenuUserWidget, SeedDisplay);
}