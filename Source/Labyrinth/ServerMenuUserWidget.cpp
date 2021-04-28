#include "ServerMenuUserWidget.h"
#include "LabyrinthGameInstance.h"

void UServerMenuUserWidget::OnConstructServer() {

	PlayMode = FText::FromString("LAN");
	buttonVisible = true;
	lan = true;
	listDisplayed = false;
	joining = false;
}

void UServerMenuUserWidget::OnClickBack() {

	RemoveFromParent();

	SessionsList.Empty();
	listDisplayed = false;
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ClearSessionSearch();
	instance->ShowMainMenu();
}

void UServerMenuUserWidget::OnClickAccept() {
	RefreshServers();
}

void UServerMenuUserWidget::OnClickToggleRightServer()
{
	if (lan) {
		PlayMode = FText::FromString("INTERNET");
		lan = !lan;
	}
}

void UServerMenuUserWidget::OnClickToggleLeftServer()
{
	if (!lan) {
		PlayMode = FText::FromString("LAN");
		lan = !lan;
	}
}

void UServerMenuUserWidget::RefreshServers() {

	buttonVisible = false;

	SessionsList.Empty();
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->FindSessions(instance->GetPrimaryPlayerUniqueId(), lan, false);
}

void UServerMenuUserWidget::NativeTick(const FGeometry& Geometry, float deltaTime)
{
	Super::NativeTick(Geometry, deltaTime);

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	TSharedPtr<FOnlineSessionSearch> search = instance->GetSessionSearch();

	if ((search && search->SearchState == EOnlineAsyncTaskState::Done) && (!listDisplayed)) {

		SessionsList = instance->GetSessionSearch().Get()->SearchResults;
		if (SessionsList.Num() != 0) {

			for (FOnlineSessionSearchResult sessionRes : SessionsList) {

				FString name = sessionRes.Session.SessionSettings.Settings.FindRef("CUSTOMSEARCHINT1").Data.ToString();
				FString places = sessionRes.Session.SessionSettings.Settings.FindRef("CUSTOMSEARCHINT2").Data.ToString();
				UpdateServerList(FText::FromString(name), 
					FText::FromString(places), 
					FText::FromString(FString::FromInt(sessionRes.PingInMs)));
				listDisplayed = true;
			}
		}
	}
}

void UServerMenuUserWidget::Join(FText ServerName) {

	auto session = SessionsList.FindByPredicate([&](FOnlineSessionSearchResult session) {
		return session.Session.SessionSettings.Settings.FindRef("CUSTOMSEARCHINT1").Data.ToString() == ServerName.ToString();
	});

	FString places = session->Session.SessionSettings.Settings.FindRef("CUSTOMSEARCHINT2").Data.ToString();
	if (session && FCString::Atoi(*places) != 0) {
		RemoveFromParent();

		ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
		FString name = session->Session.SessionSettings.Settings.FindRef("CUSTOMSEARCHINT1").Data.ToString();
		//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, FString::Printf(TEXT("LE NOM DU SERVEUR : %s"), *name));
		instance->JoinServer(FName(name), *session);


		auto updated = session->Session.SessionSettings;
		updated.Set(SETTING_CUSTOMSEARCHINT2, FString::FromInt(FCString::Atoi(*places) - 1), EOnlineDataAdvertisementType::ViaOnlineService);
		Online::GetSessionInterface()->UpdateSession(FName(name), updated);
	}
}