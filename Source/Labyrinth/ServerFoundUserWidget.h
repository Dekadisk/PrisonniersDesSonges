#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerFoundUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UServerFoundUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FText ServerName;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FText NbPlayers;

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	FText Ping;

	UFUNCTION(BlueprintCallable, Category = "ServerFound")
	void OnClickedJoin();

	UPROPERTY(BlueprintReadWrite, meta = (ExposeOnSpawn = "true"))
	UUserWidget* serverMenu;

};
