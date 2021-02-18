#include "ServerMenuUserWidget.h"
#include "LabyrinthGameInstance.h"

void UServerMenuUserWidget::OnTextChangedIpAddress(FText ip) {

	EnteredIpAddress = ip;
	accept = !ip.IsEmpty();
}

void UServerMenuUserWidget::OnClickBack() {

	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}

void UServerMenuUserWidget::OnClickAccept() {

		RemoveFromParent();

		ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
		instance->JoinServer(EnteredIpAddress);
		//GetWorld()->URL.
}

/*void UServerMenuUserWidget::DisplaySession() {

	if (sessionFound) {

	}
}*/