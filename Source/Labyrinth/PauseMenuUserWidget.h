#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerInfo.h"
#include "PauseMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UPauseMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnConstructPause();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickResume();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickQuit();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickOptions();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickRightShadow();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickLeftShadow();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickRightTexture();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickLeftTexture();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickRightPost();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickLeftPost();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickRightLanguage();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickLeftLanguage();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickLeftResolution();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickRightResolution();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickBackOptions();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnClickAcceptOptions();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	void OnCheckStateChanged(bool checked);

	// Bind

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	FText BindShadowQuality();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	FText BindTextureQuality();

	UFUNCTION(BlueprintCallable, Category = "PauseMenu")
	FText BindPostQuality();

	UPROPERTY(BlueprintReadWrite, Category = "PauseMenu")
	ESlateVisibility OptionsVisible = ESlateVisibility::Hidden;

	UPROPERTY(BlueprintReadWrite, Category = "PauseMenu")
	ESlateVisibility PauseVisible = ESlateVisibility::Visible;

	UPROPERTY(BlueprintReadWrite, Category = "PauseMenu")
	FText Language;

	UPROPERTY(BlueprintReadWrite, Category = "PauseMenu")
	FText Resolution;

	UPROPERTY(BlueprintReadWrite, Category = "PauseMenu")
	bool Fullscreen;

private:

	UPROPERTY()
	FText ShadowQuality;

	FText ShadowPrint;

	UPROPERTY()
	FText TextureQuality;

	FText TexturePrint;

	UPROPERTY()
	FText PostQuality;

	FText PostPrint;

	void UpdateOptions();

	void SaveGame();

};
