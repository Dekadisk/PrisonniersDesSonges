#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
#include <Runtime/UMG/Public/Components/WidgetSwitcher.h>
#include "ServerMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UServerMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnConstructServer();

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnClickBack();

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnClickAccept();

	//UFUNCTION(BlueprintCallable, Category = "Session")
	void DisplaySession(FOnlineSessionSearchResult session);

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	bool GetAccept() { return accept; }

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnClickToggleRightServer();

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnClickToggleLeftServer();

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	FText GetPlayModeH() { return PlayModeH; }

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	FText GetPlayMode() { return PlayMode; }

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	bool GetButtonVisibility() { return buttonVisible; }

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ServerMenu", meta = (BindWidget))
	UWidgetSwitcher* switchLoad;

	virtual void NativeTick(const FGeometry& Geometry, float deltaTime) override;

private:

	UPROPERTY()
	FText PlayMode;
	bool lan;

	UPROPERTY()
	FText PlayModeH;

	bool sessionFound;
	FOnlineSessionSearchResult SessionAvailable;
	TArray<FOnlineSessionSearchResult> SessionsList;

	bool accept;
	bool buttonVisible;

	void RefreshServers();
};
