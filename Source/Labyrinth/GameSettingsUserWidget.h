#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GameSettingsUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UGameSettingsUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "GameSettingsMenu")
	void OnClickAccept();

	UFUNCTION(BlueprintCallable, Category = "GameSettingsMenu")
	void OnClickBack();
};
