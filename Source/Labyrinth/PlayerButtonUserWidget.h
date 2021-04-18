#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "PlayerInfo.h"
#include "PlayerButtonUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UPlayerButtonUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable, Category = "OnClick")
	void OnClickPlayer();

	UPROPERTY(Replicated)
	int id;

	UPROPERTY(Replicated)
	FPlayerInfo playerInfo;

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
