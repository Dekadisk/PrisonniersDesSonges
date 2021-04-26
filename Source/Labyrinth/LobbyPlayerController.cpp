#include "LobbyPlayerController.h"
#include "LobbyGameMode.h"
#include "LabyrinthGameInstance.h"
#include "Kismet/GameplayStatics.h"

ALobbyPlayerController::ALobbyPlayerController() {
	static ConstructorHelpers::FClassFinder<UUserWidget> LobbyMenuWidget{ TEXT("/Game/UI/LobbyMenu") };
	LobbyMenuWidgetClass = LobbyMenuWidget.Class;
	bReplicates = true;

	PlayerSettingsSaved = "PlayerSettingsSaved";
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
		for (int i = 0; i < connectedPlayerInfo.Num(); ++i)
			LobbyMenu->UpdatePlayerWindow(connectedPlayerInfo[i], i);
	}
}

void ALobbyPlayerController::UpdateLocalSettings_Implementation(int seed)
{
	if (seed != 0)
		partySeed.Emplace(seed);
	else
		partySeed.Reset();

	LobbyMenu->UpdateSeedDisplay(FText::FromString(FString::FromInt(seed)));
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

void ALobbyPlayerController::ServerGetChatMsg_Implementation(const FText& textToSend) {

	senderText = textToSend;
	senderName = playerSettings.PlayerName;
	TArray<APlayerController*> pcs = Cast<ALobbyGameMode>(GetWorld()->GetAuthGameMode())->AllPlayerControllers;

	for (APlayerController* pc : pcs)
		Cast<ALobbyPlayerController>(pc)->UpdateChat(senderName, senderText);
}

void ALobbyPlayerController::UpdateChat_Implementation(const FText& sender, const FText& text) {
	LobbyMenu->ChatWindow->UpdateChatWindow(sender, text);
}

void ALobbyPlayerController::SetupLobbyMenu_Implementation(const FName &ServerName) {

	SetShowMouseCursor(true);

	LobbyMenu = CreateWidget<ULobbyMenuUserWidget>(this, LobbyMenuWidgetClass);
	LobbyMenu->ServerName = ServerName;
	LobbyMenu->AddToViewport();
}

void ALobbyPlayerController::TravelToLvl_Implementation() {
	ClientTravel("/Game/procedural_level", ETravelType::TRAVEL_Relative);
}

void ALobbyPlayerController::SaveSeed_Implementation(int seed) {
	Cast<ULabyrinthGameInstance>(GetGameInstance())->seed = seed;
}

void ALobbyPlayerController::Kicked_Implementation()
{
	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));

	ULabyrinthGameInstance* lobbyGameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());
	lobbyGameInst->DestroySession(lobbyGameInst->SessionName);
}

void ALobbyPlayerController::SaveGameCheck()
{
	if (UGameplayStatics::DoesSaveGameExist(PlayerSettingsSaved, 0))
		LoadGame();

	SaveGame();
}

void ALobbyPlayerController::SaveGame() {
	
	if (!save->IsValidLowLevel())
		save = Cast<UPlayerSaveGame>(UGameplayStatics::CreateSaveGameObject(UPlayerSaveGame::StaticClass()));

	save->SetPlayerInfo(playerSettings);
	UGameplayStatics::SaveGameToSlot(save, PlayerSettingsSaved, 0);
}

void ALobbyPlayerController::LoadGame()
{
	UPlayerSaveGame* loadedSave = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSaved, 0));
	playerSettings = loadedSave->GetPlayerInfo();
}


void ALobbyPlayerController::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	if (IsLocalController() && !(EEndPlayReason::Destroyed == reason))
	{
		ULabyrinthGameInstance* lobbyGameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());
		if (IsValid(lobbyGameInst))
			lobbyGameInst->DestroySession(lobbyGameInst->SessionName);
	}
}

void ALobbyPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALobbyPlayerController, allConnectedPlayers);
	DOREPLIFETIME(ALobbyPlayerController, playerSettings);
}
