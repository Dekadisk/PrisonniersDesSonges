#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"
#include "ChatTextUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UChatTextUserWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UPROPERTY(BlueprintReadWrite, Replicated, meta = (ExposeOnSpawn = "true"))
	FText text;

	//multi
	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
