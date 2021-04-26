#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Online.h"
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

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	bool GetAccept() { return accept; }

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnClickToggleRightServer();

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	void OnClickToggleLeftServer();

	UFUNCTION(BlueprintCallable, Category = "ServerMenu")
	bool GetButtonVisibility() { return buttonVisible; }

	UFUNCTION(BlueprintImplementableEvent, Category = "ServerMenu")
	void UpdateServerList(const FText& ServerName, const FText& NbPlayers, const FText& ping);

	UPROPERTY(BlueprintReadWrite, Category = "ServerMenu")
	FText PlayMode;

	virtual void NativeTick(const FGeometry& Geometry, float deltaTime) override;

	void Join(FText ServerName);

private:

	bool lan;

	bool sessionFound;
	FOnlineSessionSearchResult SessionAvailable;
	TArray<FOnlineSessionSearchResult> SessionsList;

	bool accept;
	bool buttonVisible;
	
	bool listDisplayed;

	bool joining;

	void RefreshServers();

};
