// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthPlayerController.h"
#include "SelectionWheelUserWidget.h"
#include "PlayerSaveGame.h"
#include "LabyrinthGameModeBase.h"
#include "InGameChatWidget.h"
#include "LabyrinthGameInstance.h"
#include "Cachette.h"

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

void ALabyrinthPlayerController::Tick(float ds) {
	
	/*int a = 1;
	a++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("xxxxxxxx"))*/
	//GEngine->AddOnScreenDebugMessage(1, 3.f, FColor::Red, chatOn ? "1" : "0");
}

void ALabyrinthPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetInputMode(FInputModeGameOnly());
	if (IsLocalController()) {
		SelectionWheel = CreateWidget<UUserWidget>(this, SelectionWheelWidgetClass);
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

	for (APlayerController* pc : pcs) {
		Cast<ALabyrinthPlayerController>(pc)->UpdateChat(senderName, senderText);
	}
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
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut EndPlay");
	Super::EndPlay(reason);

	if (IsLocalController())
	{

		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "EH OH CA DEGAGE");
		ULabyrinthGameInstance* GameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());
		if (IsValid(GameInst))
		{
			GameInst->DestroySession(GameInst->SessionName);
		}
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
}
