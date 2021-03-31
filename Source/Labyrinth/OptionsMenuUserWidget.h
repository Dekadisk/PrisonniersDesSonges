#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "Kismet/GameplayStatics.h"
#include "OptionsMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UOptionsMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnConstructOptions();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnTextChangedUserPlayerName(FText name);

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickBackOptions();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickAcceptOptions();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	FText GetEnteredPlayerName() { return EnteredPlayerName; }

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	bool GetAccept() { return accept; }

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	bool BackVisible() { return saveFound; }

private:

	UPROPERTY()
	FPlayerInfo playerInfo;

	UPROPERTY()
	FText EnteredPlayerName;

	UPROPERTY()
	bool accept;

	UPROPERTY()
	FString PlayerSettingsSaved;

	bool saveFound;

	void SaveGameCheck();

	void EmptyNameCheck(FText name);

	void SaveGame();

	void LoadGame();
	
};
