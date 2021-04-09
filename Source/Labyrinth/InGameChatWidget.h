#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ChatWindowUserWidget.h"
#include "InGameChatWidget.generated.h"

UCLASS()
class LABYRINTH_API UInGameChatWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "InGameChat", meta = (BindWidget))
	UChatWindowUserWidget* chatWindow;
	
};
