#include "HostMenuUserWidget.h"
#include "LabyrinthGameInstance.h"

void UHostMenuUserWidget::OnConstructHost() {
	PlayMode = FText::FromString("LAN");
	lan = true;
}

void UHostMenuUserWidget::OnTextChangedServerName(FText name) {

	ServerName = FName(name.ToString());
	accept = !name.IsEmpty();

}

void UHostMenuUserWidget::OnClickBackHost()
{
	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UHostMenuUserWidget::OnClickAcceptHost()
{
	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->LaunchLobby(4, lan, ServerName);
}

void UHostMenuUserWidget::OnClickToggleRightHost()
{
	if (lan) {
		PlayMode = FText::FromString("INTERNET");
		lan = !lan;
	}
}

void UHostMenuUserWidget::OnClickToggleLeftHost()
{
	if (!lan) {
		PlayMode = FText::FromString("LAN");
		lan = !lan;
	}
}
