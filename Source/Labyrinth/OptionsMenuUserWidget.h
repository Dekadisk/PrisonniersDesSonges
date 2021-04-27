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
	void OnClickRightShadow();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickLeftShadow();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickRightTexture();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickLeftTexture();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickRightPost();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickLeftPost();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickRightLanguage();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickLeftLanguage();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickLeftResolution();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickRightResolution();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickBackOptions();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickAcceptOptions();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnClickChangeName();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	void OnCheckStateChanged(bool checked);

	// Bind

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	FText BindShadowQuality();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	FText BindTextureQuality();

	UFUNCTION(BlueprintCallable, Category = "OptionsMenu")
	FText BindPostQuality();

	UPROPERTY(BlueprintReadWrite, Category = "OptionsMenu")
	FText Language;

	UPROPERTY(BlueprintReadWrite, Category = "OptionsMenu")
	FText Resolution;

	UPROPERTY(BlueprintReadWrite, Category = "OptionsMenu")
	bool Fullscreen;

private:

	FPlayerInfo playerInfo;

	FText ShadowQuality;
	FText ShadowPrint;

	FText TextureQuality;
	FText TexturePrint;

	FText PostQuality;
	FText PostPrint;

	FString PlayerSettingsSaved;

	bool saveFound;

	void SaveGameCheck();

	void SaveGame();

	void LoadGame();

	void UpdateOptions();
	
};
