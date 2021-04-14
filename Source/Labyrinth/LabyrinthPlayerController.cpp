// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthPlayerController.h"
#include "SelectionWheelUserWidget.h"
#include "PlayerSaveGame.h"
#include "LabyrinthGameModeBase.h"
#include "InGameChatWidget.h"

ALabyrinthPlayerController::ALabyrinthPlayerController()
{
	bHasKey = false;
	bHasLantern = false;
	bHasTrap = false;
	bHasChalk = false;
	bIsInCupboard = false;
	bIsHidden = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> SelectionWheelWidget{ TEXT("/Game/UI/SelectionWheel") };
	SelectionWheelWidgetClass = SelectionWheelWidget.Class;

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialSW(TEXT("/Game/Assets/SelectionWheel/SW.SW"));

	if (FoundMaterialSW.Succeeded()) SelectionWheelMaterial = FoundMaterialSW.Object;

	static ConstructorHelpers::FClassFinder<UUserWidget> ChatUserWidget{ TEXT("/Game/UI/GameplayChat") };
	ChatWidgetClass = ChatUserWidget.Class;

	PlayerSettingsSaved = "PlayerSettingsSaved";
}

void ALabyrinthPlayerController::Tick(float ds) {
	/*int a = 1;
	a++;
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("xxxxxxxx"));*/
}

void ALabyrinthPlayerController::BeginPlay()
{
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
	Cast<UInGameChatWidget>(ChatWidget)->chatWindow->UpdateChatWindow(sender, text);
}

void ALabyrinthPlayerController::ServerGetPlayerInfo_Implementation(FPlayerInfo playerSettingsInfo) {
	playerSettings = playerSettingsInfo;
	SetupChatWindow();
}

void ALabyrinthPlayerController::LoadGame() {
	UPlayerSaveGame* save = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSaved, 0));
	playerSettings = save->GetPlayerInfo();
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
}