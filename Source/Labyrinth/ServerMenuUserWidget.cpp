#include "ServerMenuUserWidget.h"
#include "LabyrinthGameInstance.h"

void UServerMenuUserWidget::OnConstructServer() {

	PlayMode = FText::FromString("LAN");
	buttonVisible = true;
	PlayModeH = FText::FromString("Choose Play Method");
	lan = true;
}

void UServerMenuUserWidget::OnClickBack() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UServerMenuUserWidget::OnClickAccept() {

	if (sessionFound) {
		RemoveFromParent();

		ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());

		// NOM A MODIFIER <-----------------------------------------------------------------------
		instance->JoinServer(FName("serv"), SessionAvailable);
	}
	else {
		RefreshServers();
	}
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
	SessionsList = instance->GetSessionSearch().Get()->SearchResults;
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
	switchLoad->SetActiveWidgetIndex(0);
}