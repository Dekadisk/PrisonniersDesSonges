#include "PauseMenuUserWidget.h"
#include "LabyrinthPlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "LabyrinthGameModeBase.h"
#include "LabyrinthGameInstance.h"

void UPauseMenuUserWidget::OnConstructPause() {

	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetOwningPlayer());

	ShadowQuality = pc->playerSettings.ShadowQuality;
	TextureQuality = pc->playerSettings.TextureQuality;
	PostQuality = pc->playerSettings.PostQuality;
	Language = pc->playerSettings.Language;
	Resolution = pc->playerSettings.Resolution;
	Fullscreen = pc->playerSettings.Fullscreen;
}

void UPauseMenuUserWidget::OnClickResume() {

	RemoveFromParent();

	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetOwningPlayer());

	pc->SetInputMode(FInputModeGameOnly());
	pc->pauseOn = false;
	pc->bShowMouseCursor = false;
}

void UPauseMenuUserWidget::OnClickOptions() {

	PauseVisible = ESlateVisibility::Hidden;
	OptionsVisible = ESlateVisibility::Visible;
}

void UPauseMenuUserWidget::OnClickRightShadow()
{
	if (ShadowQuality.ToString() == "0")
		ShadowQuality = FText::FromString("1");
	else if (ShadowQuality.ToString() == "1")
		ShadowQuality = FText::FromString("2");
}

void UPauseMenuUserWidget::OnClickLeftShadow()
{
	if (ShadowQuality.ToString() == "2")
		ShadowQuality = FText::FromString("1");
	else if (ShadowQuality.ToString() == "1")
		ShadowQuality = FText::FromString("0");
}

void UPauseMenuUserWidget::OnClickRightTexture()
{
	if (TextureQuality.ToString() == "0")
		TextureQuality = FText::FromString("1");
	else if (TextureQuality.ToString() == "1")
		TextureQuality = FText::FromString("2");
}

void UPauseMenuUserWidget::OnClickLeftTexture()
{
	if (TextureQuality.ToString() == "2")
		TextureQuality = FText::FromString("1");
	else if (TextureQuality.ToString() == "1")
		TextureQuality = FText::FromString("0");
}

void UPauseMenuUserWidget::OnClickRightPost()
{
	if (PostQuality.ToString() == "0")
		PostQuality = FText::FromString("1");
	else if (PostQuality.ToString() == "1")
		PostQuality = FText::FromString("2");
}

void UPauseMenuUserWidget::OnClickLeftPost()
{
	if (PostQuality.ToString() == "2")
		PostQuality = FText::FromString("1");
	else if (PostQuality.ToString() == "1")
		PostQuality = FText::FromString("0");
}

void UPauseMenuUserWidget::OnClickRightLanguage()
{
	if (Language.ToString() == "Francais")
		Language = FText::FromString("English");
}

void UPauseMenuUserWidget::OnClickLeftLanguage()
{
	if (Language.ToString() == "English")
		Language = FText::FromString("Francais");
}

void UPauseMenuUserWidget::OnClickLeftResolution()
{
	if (Resolution.ToString() == "1920x1080")
		Resolution = FText::FromString("1280x720");
	else if (Resolution.ToString() == "1280x720")
		Resolution = FText::FromString("800x600");
}

void UPauseMenuUserWidget::OnClickRightResolution()
{
	if (Resolution.ToString() == "800x600")
		Resolution = FText::FromString("1280x720");
	else if (Resolution.ToString() == "1280x720")
		Resolution = FText::FromString("1920x1080");
}

void UPauseMenuUserWidget::OnClickBackOptions() {

	OptionsVisible = ESlateVisibility::Hidden;
	PauseVisible = ESlateVisibility::Visible;
}

void UPauseMenuUserWidget::UpdateOptions() {

	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetOwningPlayer());

	if (ShadowQuality.ToString() != pc->playerSettings.ShadowQuality.ToString()) {

		pc->playerSettings.ShadowQuality = ShadowQuality;
		FString exe = "sg.ShadowQuality " + ShadowQuality.ToString();
		pc->ConsoleCommand(exe);
	}

	if (TextureQuality.ToString() != pc->playerSettings.TextureQuality.ToString()) {

		pc->playerSettings.TextureQuality = TextureQuality;
		FString exe = "sg.TextureQuality " + TextureQuality.ToString();
		pc->ConsoleCommand(exe);
	}

	if (PostQuality.ToString() != pc->playerSettings.PostQuality.ToString()) {

		pc->playerSettings.PostQuality = PostQuality;
		FString exe = "sg.PostProcessQuality " + PostQuality.ToString();
		pc->ConsoleCommand(exe);
	}

	if (Language.ToString() != pc->playerSettings.Language.ToString()) {

		pc->playerSettings.Language = Language;
		FString exe = "sg.Language " + Language.ToString();
		// CHANGER LANGUE <--------------------------------------------------------
	}

	if (Resolution.ToString() != pc->playerSettings.Resolution.ToString()) {

		pc->playerSettings.Resolution = Resolution;
		FString exe = "r.setRes " + Resolution.ToString() + "f";
		pc->ConsoleCommand(exe);
	}
}
void UPauseMenuUserWidget::OnClickAcceptOptions() {

	UpdateOptions();

	SaveGame();

	OptionsVisible = ESlateVisibility::Hidden;
	PauseVisible = ESlateVisibility::Visible;
}

void UPauseMenuUserWidget::OnCheckStateChanged(bool checked)
{
	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetOwningPlayer());

	auto settings = GEngine->GetGameUserSettings();
	if (checked) {
		settings->SetFullscreenMode(EWindowMode::Fullscreen);
		Fullscreen = true;
		pc->playerSettings.Fullscreen = true;
	}
	else {
		settings->SetFullscreenMode(EWindowMode::Windowed);
		Fullscreen = false;
		pc->playerSettings.Fullscreen = false;
	}

	settings->ApplySettings(true);
}

FText UPauseMenuUserWidget::BindShadowQuality()
{
	if (Language.ToString() == "English")
		return ShadowQuality.ToString() == "0" ? FText::FromString("Low") : ShadowQuality.ToString() == "1" ? FText::FromString("Medium") : FText::FromString("High");
	else
		return ShadowQuality.ToString() == "0" ? FText::FromString("Faible") : ShadowQuality.ToString() == "1" ? FText::FromString("Moyen") : FText::FromString("Eleve");
}

FText UPauseMenuUserWidget::BindTextureQuality()
{
	if (Language.ToString() == "English")
		return TextureQuality.ToString() == "0" ? FText::FromString("Low") : TextureQuality.ToString() == "1" ? FText::FromString("Medium") : FText::FromString("High");
	else
		return TextureQuality.ToString() == "0" ? FText::FromString("Faible") : TextureQuality.ToString() == "1" ? FText::FromString("Moyen") : FText::FromString("Eleve");
}

FText UPauseMenuUserWidget::BindPostQuality()
{
	if (Language.ToString() == "English")
		return PostQuality.ToString() == "0" ? FText::FromString("Low") : PostQuality.ToString() == "1" ? FText::FromString("Medium") : FText::FromString("High");
	else
		return PostQuality.ToString() == "0" ? FText::FromString("Faible") : PostQuality.ToString() == "1" ? FText::FromString("Moyen") : FText::FromString("Eleve");
}

void UPauseMenuUserWidget::SaveGame() {

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	ALabyrinthPlayerController* pc = Cast<ALabyrinthPlayerController>(GetOwningPlayer());

	instance->GetSaveFile()->SetPlayerInfo(pc->playerSettings);
	UGameplayStatics::SaveGameToSlot(instance->GetSaveFile(), pc->PlayerSettingsSaved, 0);
}

void UPauseMenuUserWidget::OnClickQuit() {
	
	RemoveFromParent();

	ALabyrinthPlayerController* owningPC = Cast<ALabyrinthPlayerController>(GetOwningPlayer());

	if (!GetWorld()->IsServer()) {
		if (IsValid(owningPC)) {
			owningPC->EndPlay(EEndPlayReason::Quit);
			UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
		}
	}
	else {
		TArray<APlayerController*> pcs = Cast<ALabyrinthGameModeBase>(GetWorld()->GetAuthGameMode())->AllPlayerControllers;
		for (APlayerController* pc : pcs) {
			if (pc->GetNetMode() == ENetMode::NM_Client)
				Cast<ALabyrinthPlayerController>(pc)->Kicked();
		}

		if (IsValid(owningPC)) {
			owningPC->EndPlay(EEndPlayReason::Quit);
			UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
		}
	}
}