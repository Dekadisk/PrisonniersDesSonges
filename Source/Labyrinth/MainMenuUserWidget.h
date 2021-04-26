#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UMainMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnConstructMain();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnClickHost();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnClickFind();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnClickOptions();

	UFUNCTION(BlueprintCallable, Category = "MainMenu")
	void OnClickQuit();
};
