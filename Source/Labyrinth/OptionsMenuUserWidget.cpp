#include "OptionsMenuUserWidget.h"
#include "LabyrinthGameInstance.h"
#include "PlayerSaveGame.h"
#include "Kismet/KismetInternationalizationLibrary.h"

void UOptionsMenuUserWidget::OnConstructOptions() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	PlayerSettingsSaved = instance->GetFileName();
	saveFound = false;
	SaveGameCheck();

	ShadowQuality = playerInfo.ShadowQuality;
	TextureQuality = playerInfo.TextureQuality;
	PostQuality = playerInfo.PostQuality;
	Language = playerInfo.Language;

	if (Language.ToString() == "English")
		ShadowPrint = ShadowQuality.ToString() == "0" ? FText::FromString("Low") : ShadowQuality.ToString() == "1" ? FText::FromString("Medium") : ShadowQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
	else
		ShadowPrint = ShadowQuality.ToString() == "0" ? FText::FromString("Faible") : ShadowQuality.ToString() == "1" ? FText::FromString("Moyen") : ShadowQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");

	if (Language.ToString() == "English")
		TexturePrint = TextureQuality.ToString() == "0" ? FText::FromString("Low") : TextureQuality.ToString() == "1" ? FText::FromString("Medium") : TextureQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
	else
		TexturePrint = TextureQuality.ToString() == "0" ? FText::FromString("Faible") : TextureQuality.ToString() == "1" ? FText::FromString("Moyen") : TextureQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");

	if (Language.ToString() == "English")
		PostPrint = PostQuality.ToString() == "0" ? FText::FromString("Low") : PostQuality.ToString() == "1" ? FText::FromString("Medium") : PostQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
	else
		PostPrint = PostQuality.ToString() == "0" ? FText::FromString("Faible") : PostQuality.ToString() == "1" ? FText::FromString("Moyen") : PostQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");

	Resolution = playerInfo.Resolution;
	Fullscreen = playerInfo.Fullscreen;
}

void UOptionsMenuUserWidget::OnClickBackOptions() {
	
	RemoveFromParent();
	
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::UpdateOptions() {

	if (ShadowQuality.ToString() != playerInfo.ShadowQuality.ToString()) {

		playerInfo.ShadowQuality = ShadowQuality;
		FString exe = "sg.ShadowQuality " + ShadowQuality.ToString();
		GetOwningLocalPlayer()->ConsoleCommand(exe);
	}

	if (TextureQuality.ToString() != playerInfo.TextureQuality.ToString()) {

		playerInfo.TextureQuality = TextureQuality;
		FString exe = "sg.TextureQuality " + TextureQuality.ToString();
		GetOwningLocalPlayer()->ConsoleCommand(exe);
	}

	if (PostQuality.ToString() != playerInfo.PostQuality.ToString()) {

		playerInfo.PostQuality = PostQuality;
		FString exe = "sg.PostProcessQuality " + PostQuality.ToString();
		GetOwningLocalPlayer()->ConsoleCommand(exe);
	}

	if (Language.ToString() != playerInfo.Language.ToString()) {

		playerInfo.Language = Language;
		FString exe = Language.ToString() == "Francais" ? "fr-FR" : "en-GB";
		UKismetInternationalizationLibrary::SetCurrentCulture(exe);
		UKismetInternationalizationLibrary::SetCurrentLanguage(exe);
	}

	if (Fullscreen != playerInfo.Fullscreen || Resolution.ToString() != playerInfo.Resolution.ToString()) {
		playerInfo.Fullscreen = Fullscreen;
		playerInfo.Resolution = Resolution;
		if (Fullscreen) {
			FString exe = "r.setRes " + Resolution.ToString() + "f";
			GetOwningLocalPlayer()->ConsoleCommand(exe);
		}
		else {
			FString exe = "r.setRes " + Resolution.ToString() + "w";
			GetOwningLocalPlayer()->ConsoleCommand(exe);
		}
	}
}
void UOptionsMenuUserWidget::OnClickAcceptOptions() {

	UpdateOptions();

	SaveGame();

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->SetFileSaved(true);
	instance->ShowMainMenu();
}

void UOptionsMenuUserWidget::OnClickChangeName()
{
	UpdateOptions();
	SaveGame();
	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowNameMenu();
}

void UOptionsMenuUserWidget::OnCheckStateChanged(bool checked)
{
	Fullscreen = checked;
}

FText UOptionsMenuUserWidget::BindShadowQuality()
{
	if (Language.ToString() == playerInfo.Language.ToString()) {
		if (Language.ToString() == "English")
			ShadowPrint = ShadowQuality.ToString() == "0" ? FText::FromString("Low") : ShadowQuality.ToString() == "1" ? FText::FromString("Medium") : ShadowQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
		else
			ShadowPrint = ShadowQuality.ToString() == "0" ? FText::FromString("Faible") : ShadowQuality.ToString() == "1" ? FText::FromString("Moyen") : ShadowQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");
	}
	else {
		if (playerInfo.Language.ToString() == "English")
			ShadowPrint = ShadowQuality.ToString() == "0" ? FText::FromString("Low") : ShadowQuality.ToString() == "1" ? FText::FromString("Medium") : ShadowQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
		else
			ShadowPrint = ShadowQuality.ToString() == "0" ? FText::FromString("Faible") : ShadowQuality.ToString() == "1" ? FText::FromString("Moyen") : ShadowQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");
	}
	return ShadowPrint;
}

FText UOptionsMenuUserWidget::BindTextureQuality()
{
	if (Language.ToString() == playerInfo.Language.ToString()) {
		if (Language.ToString() == "English")
			TexturePrint = TextureQuality.ToString() == "0" ? FText::FromString("Low") : TextureQuality.ToString() == "1" ? FText::FromString("Medium") : TextureQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
		else
			TexturePrint = TextureQuality.ToString() == "0" ? FText::FromString("Faible") : TextureQuality.ToString() == "1" ? FText::FromString("Moyen") : TextureQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");
	}
	else {
		if (playerInfo.Language.ToString() == "English")
			TexturePrint = TextureQuality.ToString() == "0" ? FText::FromString("Low") : TextureQuality.ToString() == "1" ? FText::FromString("Medium") : TextureQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
		else
			TexturePrint = TextureQuality.ToString() == "0" ? FText::FromString("Faible") : TextureQuality.ToString() == "1" ? FText::FromString("Moyen") : TextureQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");
	}
	return TexturePrint;
}

FText UOptionsMenuUserWidget::BindPostQuality()
{
	if (Language.ToString() == playerInfo.Language.ToString()) {
		if (Language.ToString() == "English")
			PostPrint = PostQuality.ToString() == "0" ? FText::FromString("Low") : PostQuality.ToString() == "1" ? FText::FromString("Medium") : PostQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
		else
			PostPrint = PostQuality.ToString() == "0" ? FText::FromString("Faible") : PostQuality.ToString() == "1" ? FText::FromString("Moyen") : PostQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");
	}
	else {
		if (playerInfo.Language.ToString() == "English")
			PostPrint = PostQuality.ToString() == "0" ? FText::FromString("Low") : PostQuality.ToString() == "1" ? FText::FromString("Medium") : PostQuality.ToString() == "2" ? FText::FromString("High") : FText::FromString("Ultra");
		else
			PostPrint = PostQuality.ToString() == "0" ? FText::FromString("Faible") : PostQuality.ToString() == "1" ? FText::FromString("Moyen") : PostQuality.ToString() == "2" ? FText::FromString("Eleve") : FText::FromString("Ultra");
	}
	return PostPrint;
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

void UOptionsMenuUserWidget::OnClickRightShadow()
{
	if (ShadowQuality.ToString() == "0")
		ShadowQuality = FText::FromString("1");
	else if (ShadowQuality.ToString() == "1")
		ShadowQuality = FText::FromString("2");
	else if (ShadowQuality.ToString() == "2")
		ShadowQuality = FText::FromString("3");
}

void UOptionsMenuUserWidget::OnClickLeftShadow()
{
	if (ShadowQuality.ToString() == "3")
		ShadowQuality = FText::FromString("2");
	else if (ShadowQuality.ToString() == "2")
		ShadowQuality = FText::FromString("1");
	else if (ShadowQuality.ToString() == "1")
		ShadowQuality = FText::FromString("0");
}

void UOptionsMenuUserWidget::OnClickRightTexture()
{
	if (TextureQuality.ToString() == "0")
		TextureQuality = FText::FromString("1");
	else if (TextureQuality.ToString() == "1")
		TextureQuality = FText::FromString("2");
	else if (TextureQuality.ToString() == "2")
		TextureQuality = FText::FromString("3");
}

void UOptionsMenuUserWidget::OnClickLeftTexture()
{
	if (TextureQuality.ToString() == "3")
		TextureQuality = FText::FromString("2");
	else if (TextureQuality.ToString() == "2")
		TextureQuality = FText::FromString("1");
	else if (TextureQuality.ToString() == "1")
		TextureQuality = FText::FromString("0");
}

void UOptionsMenuUserWidget::OnClickRightPost()
{
	if (PostQuality.ToString() == "0")
		PostQuality = FText::FromString("1");
	else if (PostQuality.ToString() == "1")
		PostQuality = FText::FromString("2");
	else if (PostQuality.ToString() == "2")
		PostQuality = FText::FromString("3");
}

void UOptionsMenuUserWidget::OnClickLeftPost()
{
	if (PostQuality.ToString() == "3")
		PostQuality = FText::FromString("2");
	else if (PostQuality.ToString() == "2")
		PostQuality = FText::FromString("1");
	else if (PostQuality.ToString() == "1")
		PostQuality = FText::FromString("0");
}

void UOptionsMenuUserWidget::OnClickRightLanguage()
{
	if (Language.ToString() == "Francais")
		Language = FText::FromString("English");
}

void UOptionsMenuUserWidget::OnClickLeftLanguage()
{
	if (Language.ToString() == "English")
		Language = FText::FromString("Francais");
}

void UOptionsMenuUserWidget::OnClickLeftResolution()
{
	if (Resolution.ToString() == "1920x1080")
		Resolution = FText::FromString("1280x720");
	else if (Resolution.ToString() == "1280x720")
		Resolution = FText::FromString("800x600");
}

void UOptionsMenuUserWidget::OnClickRightResolution()
{
	if (Resolution.ToString() == "800x600")
		Resolution = FText::FromString("1280x720");
	else if (Resolution.ToString() == "1280x720")
		Resolution = FText::FromString("1920x1080");
}
