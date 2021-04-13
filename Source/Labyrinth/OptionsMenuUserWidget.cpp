#include "OptionsMenuUserWidget.h"
#include "LabyrinthGameInstance.h"
#include "PlayerSaveGame.h"

void UOptionsMenuUserWidget::OnConstructOptions() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	PlayerSettingsSaved = instance->GetFileName();
	saveFound = false;
	SaveGameCheck();
	EnteredPlayerName = playerInfo.PlayerName;
	EmptyNameCheck(EnteredPlayerName);
}

void UOptionsMenuUserWidget::OnTextChangedUserPlayerName(FText name) {

	EmptyNameCheck(name);
}

void UOptionsMenuUserWidget::OnClickBackOptions() {
	
	RemoveFromParent();
	
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::OnClickAcceptOptions() {

	playerInfo.PlayerName = EnteredPlayerName;

	SaveGame();

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->SetFileSaved(true);
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::SaveGameCheck() {

	if (UGameplayStatics::DoesSaveGameExist(PlayerSettingsSaved, 0)) {
		LoadGame();
	}
	else {
		SaveGame();
		saveFound = false;
	}
}

void UOptionsMenuUserWidget::EmptyNameCheck(FText name)
{
	EnteredPlayerName = name;
	accept = !name.IsEmpty();
}

void UOptionsMenuUserWidget::SaveGame() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->GetSaveFile()->SetPlayerInfo(playerInfo);
	UGameplayStatics::SaveGameToSlot(instance->GetSaveFile(), PlayerSettingsSaved, 0);
}

void UOptionsMenuUserWidget::LoadGame()
{
	UPlayerSaveGame* save = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSaved, 0));
	playerInfo = save->GetPlayerInfo();
	saveFound = !playerInfo.PlayerName.IsEmpty();
}
