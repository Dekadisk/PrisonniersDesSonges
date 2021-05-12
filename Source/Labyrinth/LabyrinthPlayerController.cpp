#include "LabyrinthPlayerController.h"
#include "PlayerSaveGame.h"
#include "LabyrinthGameModeBase.h"
#include "InGameChatWidget.h"
#include "LabyrinthGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include <Labyrinth\IngameScoreboard.h>

ALabyrinthPlayerController::ALabyrinthPlayerController()
{
	bHasKey = false;
	bHasLantern = false;
	bHasTrap = false;
	bHasChalk = false;
	bIsInCupboard = false;
	bIsHidden = false;
	bIsDead = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> SelectionWheelWidget{ TEXT("/Game/UI/SelectionWheel") };
	SelectionWheelWidgetClass = SelectionWheelWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> IngameScoreboardWidget{ TEXT("/Game/UI/IngameScoreboard") };
	IngameScoreboardWidgetClass = IngameScoreboardWidget.Class;

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialSW(TEXT("/Game/Assets/SelectionWheel/SW.SW"));

	if (FoundMaterialSW.Succeeded()) SelectionWheelMaterial = FoundMaterialSW.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> ChatUserWidget{ TEXT("/Game/UI/GameplayChat") };
	ChatWidgetClass = ChatUserWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseUserWidget{ TEXT("/Game/UI/PauseMenu") };
	PauseWidgetClass = PauseUserWidget.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> DeathUserWidget{ TEXT("/Game/UI/DeathScreen") };
	DeathWidgetClass = DeathUserWidget.Class;

	PlayerSettingsSaved = "PlayerSettingsSaved";
}

void ALabyrinthPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	if (IsLocalController()) {
		SelectionWheel = CreateWidget<UUserWidget>(this, SelectionWheelWidgetClass);
		Scoreboard = CreateWidget<UUserWidget>(this, IngameScoreboardWidgetClass);
		Cast<UIngameScoreboard>(Scoreboard)->owner = this;
		LoadGame();
		ServerGetPlayerInfo(playerSettings);
	}

}

void ALabyrinthPlayerController::SetupChatWindow_Implementation()
{
	ChatWidget = CreateWidget<UUserWidget>(this, ChatWidgetClass);
	ChatWidget->SetVisibility(ESlateVisibility::Hidden);
	ChatWidget->AddToViewport();
}

void ALabyrinthPlayerController::ServerGetChatMsg_Implementation(const FText& text) {
	senderText = text;
	senderName = playerSettings.PlayerName;

	TArray<APlayerController*> pcs = Cast<ALabyrinthGameModeBase>(GetWorld()->GetAuthGameMode())->AllPlayerControllers;

	for (APlayerController* pc : pcs)
		Cast<ALabyrinthPlayerController>(pc)->UpdateChat(senderName, senderText);

}

void ALabyrinthPlayerController::UpdateChat_Implementation(const FText& sender, const FText& text) {
	if (sender.ToString() != senderName.ToString() && !chatOn) {
		ChatWidget->SetVisibility(ESlateVisibility::Visible);
		GetWorld()->GetTimerManager().ClearTimer(timerChatHandle);
		GetWorld()->GetTimerManager().SetTimer(timerChatHandle, this, &ALabyrinthPlayerController::HideChat, 4, false);
	}

	Cast<UInGameChatWidget>(ChatWidget)->chatWindow->UpdateChatWindow(sender, text);
}

void ALabyrinthPlayerController::HideChat() {
	ChatWidget->SetVisibility(ESlateVisibility::Hidden);
}

void ALabyrinthPlayerController::ServerGetPlayerInfo_Implementation(FPlayerInfo playerSettingsInfo) {
	playerSettings = playerSettingsInfo;
	SetupChatWindow();
}

void ALabyrinthPlayerController::Kicked_Implementation()
{
	ULabyrinthGameInstance* GameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());

	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));

	GameInst->DestroySession(GameInst->SessionName);
}

void ALabyrinthPlayerController::ShowPauseMenu() {

	PauseWidget = CreateWidget<UUserWidget>(this, PauseWidgetClass);

	PauseWidget->AddToViewport();

	SetInputMode(FInputModeUIOnly());
}

void ALabyrinthPlayerController::ServerGetPlayersInfo_Implementation() {
	
	playersNames.Empty();
	playersInventories1.Empty();
	playersInventories2.Empty();
	playersInventories3.Empty();
	playersInventories4.Empty();

	TArray<TArray<bool>> allInventories{};

	ALabyrinthGameModeBase* gmb = Cast<ALabyrinthGameModeBase>(UGameplayStatics::GetGameMode(GetPawn()));
	TArray<APlayerController*> apc = gmb->AllPlayerControllers;
	for (int i = 0; i < apc.Num(); i++) {

		playersNames.Add(Cast<ALabyrinthPlayerController>(apc[i])->playerSettings.PlayerName);

		TArray<bool> stats;

		stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasLantern);
		stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasChalk);
		stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasKey);
		stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bHasTrap);
		stats.Add((Cast<ALabyrinthPlayerController>(apc[i]))->bIsDead);

		allInventories.Add(stats);
	}

	if (apc.Num() >= 1) playersInventories1 = allInventories[0];
	if (apc.Num() >= 2) playersInventories2 = allInventories[1];
	if (apc.Num() >= 3) playersInventories3 = allInventories[2];
	if (apc.Num() >= 4) playersInventories4 = allInventories[3];
}

void ALabyrinthPlayerController::ShowDeathScreen_Implementation() {

	DeathWidget = CreateWidget<UUserWidget>(this, DeathWidgetClass);

	DeathWidget->AddToViewport();

	DisableInput(this);
}

void ALabyrinthPlayerController::LoadGame() {
	UPlayerSaveGame* save = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSaved, 0));
	playerSettings = save->GetPlayerInfo();
}

void ALabyrinthPlayerController::EndPlay(EEndPlayReason::Type reason)
{
	Super::EndPlay(reason);

	if (IsLocalController())
	{
		ULabyrinthGameInstance* GameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());

		if (IsValid(GameInst))
			GameInst->DestroySession(GameInst->SessionName);
	}

	GetWorld()->GetTimerManager().ClearTimer(timerChatHandle);
}

void ALabyrinthPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALabyrinthPlayerController, bHasKey);
	DOREPLIFETIME(ALabyrinthPlayerController, bHasLantern);
	DOREPLIFETIME(ALabyrinthPlayerController, bHasTrap);
	DOREPLIFETIME(ALabyrinthPlayerController, bHasChalk);
	DOREPLIFETIME(ALabyrinthPlayerController, senderText);
	DOREPLIFETIME(ALabyrinthPlayerController, senderName);
	DOREPLIFETIME(ALabyrinthPlayerController, playerSettings);
	DOREPLIFETIME(ALabyrinthPlayerController, bIsHidden);
	DOREPLIFETIME(ALabyrinthPlayerController, bIsInCupboard);
	DOREPLIFETIME(ALabyrinthPlayerController, bIsDead);
	DOREPLIFETIME(ALabyrinthPlayerController, pLantern);
	DOREPLIFETIME(ALabyrinthPlayerController, playersNames);
	DOREPLIFETIME(ALabyrinthPlayerController, playersInventories1);
	DOREPLIFETIME(ALabyrinthPlayerController, playersInventories2);
	DOREPLIFETIME(ALabyrinthPlayerController, playersInventories3);
	DOREPLIFETIME(ALabyrinthPlayerController, playersInventories4);
}
