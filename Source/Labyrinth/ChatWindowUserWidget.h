#pragma once

#include "Core.h"
#include "Blueprint/UserWidget.h"
#include "ChatWindowUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UChatWindowUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintImplementableEvent, Category = "Chat")
	void UpdateChatWindow(const FText& sender, const FText& text);

	UFUNCTION(BlueprintCallable, Category = "Chat")
	void OnTextCommitedChat(FText text, ETextCommit::Type commitMethod);

};
