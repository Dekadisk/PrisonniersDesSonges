#include "ServerMenuUserWidget.h"
#include "LabyrinthGameInstance.h"
#include "ServerFoundUserWidget.h"

void UServerMenuUserWidget::OnConstructServer() {

	PlayMode = FText::FromString("LAN");
	buttonVisible = true;
	PlayModeH = FText::FromString("Choose Play Method");
	lan = true;

	//static ConstructorHelpers::FClassFinder<UUserWidget> ServerFoundWidget{ TEXT("/Game/UI/ServerFound") };
	//ServerFoundWidgetClass = ServerFoundWidget.Class;
}

void UServerMenuUserWidget::OnClickBack() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
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


void UServerMenuUserWidget::DisplaySession(FOnlineSessionSearchResult session) {

	SessionAvailable = session;
}

void UServerMenuUserWidget::RefreshServers() {
	buttonVisible = false;
	PlayModeH = FText::FromString("Searching...");

	switchLoad->SetActiveWidgetIndex(1);
	SessionsList.Empty();
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->FindSessions(instance->GetPrimaryPlayerUniqueId(), lan, false);
	/*SessionsList = instance->GetSessionSearch().Get()->SearchResults;
	if (SessionsList.Num() == 0) {
		PlayModeH = FText::FromString("Search Failed.");
	}
	else {
		for (FOnlineSessionSearchResult sessionRes : SessionsList) {
			if (sessionRes.Session.NumOpenPublicConnections != 0) {
				sessionFound = true;
				SessionAvailable = sessionRes;
				break;
			}
		}
		if (!sessionFound) {
			PlayModeH = FText::FromString("No Session Found.");
		}
		else {
			switchLoad->SetActiveWidgetIndex(1);
			DisplaySession(SessionAvailable);
		}
	}
	buttonVisible = true;
	switchLoad->SetActiveWidgetIndex(0);*/
}

void UServerMenuUserWidget::NativeTick(const FGeometry& Geometry, float deltaTime)
{
	Super::NativeTick(Geometry, deltaTime);
	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	TSharedPtr<FOnlineSessionSearch> search = instance->GetSessionSearch();
	if ((search && search->SearchState == EOnlineAsyncTaskState::Done) && (!listDisplayed)) {
		//if (sessionFound) {
		//	RemoveFromParent();

		//	// NOM A MODIFIER <-----------------------------------------------------------------------
		//	instance->JoinServer(FName(SessionAvailable.Session.OwningUserName), SessionAvailable);

		//	sessionFound = false;
		//}
		//else {
		SessionsList = instance->GetSessionSearch().Get()->SearchResults;
		if (SessionsList.Num() == 0) {
			PlayModeH = FText::FromString("Search Failed.");
		}
		else {
			for (FOnlineSessionSearchResult sessionRes : SessionsList) {
				/*if (sessionRes.Session.NumOpenPublicConnections != 0) {
					sessionFound = true;
					SessionAvailable = sessionRes;
					break;
				}*/
				FString name = sessionRes.Session.SessionSettings.Settings.FindRef("SESSIONNAME").Data.ToString();
				UpdateServerList(FText::FromString(name), 
					FText::FromString(FString::FromInt(sessionRes.Session.NumOpenPublicConnections)), 
					FText::FromString(FString::FromInt(sessionRes.PingInMs)));
				listDisplayed = true;
			}
			if (!sessionFound) {
				PlayModeH = FText::FromString("No Session Found.");
			}
			else {
				switchLoad->SetActiveWidgetIndex(1);
				DisplaySession(SessionAvailable);
			}
		}
	}
}

void UServerMenuUserWidget::Join(FText ServerName) {

	auto session = SessionsList.FindByPredicate([&](FOnlineSessionSearchResult session) {
		return session.Session.SessionSettings.Settings.FindRef("SESSIONNAME").Data.ToString() == ServerName.ToString();
	});

	if (session && session->Session.NumOpenPublicConnections != 0) {
		RemoveFromParent();

		ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
		FString name = SessionAvailable.Session.SessionSettings.Settings.FindRef("SESSIONNAME").Data.ToString();
		instance->JoinServer(FName(name), *session);
	}
}
