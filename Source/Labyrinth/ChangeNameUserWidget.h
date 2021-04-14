#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "Kismet/GameplayStatics.h"
#include "ChangeNameUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UChangeNameUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
		void OnConstructName();

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
		void OnTextChangedUserPlayerName(FText name);

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
		void OnClickBackName();

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
		void OnClickAcceptName();

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
		FText GetEnteredPlayerName() { return EnteredPlayerName; }

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
		bool GetAccept() { return accept; }

	UFUNCTION(BlueprintCallable, Category = "ChangeName")
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
