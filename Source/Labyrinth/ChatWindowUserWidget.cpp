#include "ChatWindowUserWidget.h"
#include "LobbyPlayerController.h"

//void UChatWindowUserWidget::UpdateChatWindow_Implementation(FText sender, FText text) {
//
//	FTextFormat fmt(FText::FromString("{sender} : {text}"));
//	FText res = FText::Format(fmt, sender, text);
//
//}

void UChatWindowUserWidget::OnTextCommitedChat(FText text, ETextCommit::Type commitMethod) {

	if (commitMethod == ETextCommit::OnEnter) {

		Cast<ALobbyPlayerController>(GetOwningPlayer())->ServerGetChatMsg(text);
	}
}