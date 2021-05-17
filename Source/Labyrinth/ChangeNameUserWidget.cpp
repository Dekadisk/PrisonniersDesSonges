#include "ChangeNameUserWidget.h"
#include "LabyrinthGameInstance.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerSaveGame.h"

void UChangeNameUserWidget::OnConstructName() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	PlayerSettingsSaved = instance->GetFileName();
	saveFound = false;
	SaveGameCheck();
	EnteredPlayerName = playerInfo.PlayerName;
	EmptyNameCheck(EnteredPlayerName);
}

void UChangeNameUserWidget::OnTextChangedUserPlayerName(FText name) {

	EmptyNameCheck(name);
}

void UChangeNameUserWidget::OnClickBackName() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UChangeNameUserWidget::OnClickAcceptName() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());

	playerInfo.PlayerName = EnteredPlayerName;
	SaveGame();
	if (saveFound)
	{
		if (!instance->IsOfflineMod())
		{
			instance->ChangeDBNameOnScoreServer(EnteredPlayerName.ToString());
			RemoveFromParent();
			instance->ShowLoadingScreen();
		}
		else
		{
			RemoveFromParent();
			instance->SetFileSaved(true);
			instance->ShowMainMenu();
		}
	}
	else
	{
		instance->LoginOnScoreServer();
		RemoveFromParent();
		instance->ShowLoadingScreen();
	}
}

void UChangeNameUserWidget::SaveGameCheck() {

	if (UGameplayStatics::DoesSaveGameExist(PlayerSettingsSaved, 0)) {
		LoadGame();
	}
	else {
		SaveGame();
		saveFound = false;
	}
}

void UChangeNameUserWidget::EmptyNameCheck(FText name)
{
	EnteredPlayerName = name;
	accept = !name.IsEmpty();
}

void UChangeNameUserWidget::SaveGame() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->GetSaveFile()->SetPlayerInfo(playerInfo);
	UGameplayStatics::SaveGameToSlot(instance->GetSaveFile(), PlayerSettingsSaved, 0);
}

void UChangeNameUserWidget::LoadGame()
{
	UPlayerSaveGame* save = Cast<UPlayerSaveGame>(UGameplayStatics::LoadGameFromSlot(PlayerSettingsSaved, 0));
	playerInfo = save->GetPlayerInfo();
	saveFound = !playerInfo.PlayerName.IsEmpty();
}
