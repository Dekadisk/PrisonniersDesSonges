#include "ChatWindowUserWidget.h"
#include "LobbyPlayerController.h"
#include "LabyrinthPlayerController.h"

void UChatWindowUserWidget::OnTextCommitedChat(FText text, ETextCommit::Type commitMethod) {

	if (commitMethod == ETextCommit::OnEnter) {

		if (Cast<ALobbyPlayerController>(GetOwningPlayer()))
			Cast<ALobbyPlayerController>(GetOwningPlayer())->ServerGetChatMsg(text);
		else if (Cast<ALabyrinthPlayerController>(GetOwningPlayer()))
			Cast<ALabyrinthPlayerController>(GetOwningPlayer())->ServerGetChatMsg(text);
	}
}