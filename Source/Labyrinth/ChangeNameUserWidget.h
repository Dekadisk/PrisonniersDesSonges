#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
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
		bool BindBackVisible() { return saveFound; }

	UPROPERTY(BlueprintReadWrite, Category = "ChangeName")
		bool accept;

	UPROPERTY(BlueprintReadWrite, Category = "ChangeName")
		FText EnteredPlayerName;

private:

	bool saveFound;

	FPlayerInfo playerInfo;

	FString PlayerSettingsSaved;

	void SaveGameCheck();

	void EmptyNameCheck(FText name);

	void SaveGame();

	void LoadGame();

};
