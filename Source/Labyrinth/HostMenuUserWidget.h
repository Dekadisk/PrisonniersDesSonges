#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HostMenuUserWidget.generated.h"

UCLASS()
class LABYRINTH_API UHostMenuUserWidget : public UUserWidget
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "HostMenu")
		void OnConstructHost();

	UFUNCTION(BlueprintCallable, Category = "HostMenu")
		void OnTextChangedServerName(FText name);

	UFUNCTION(BlueprintCallable, Category = "HostMenu")
		void OnClickBackHost();

	UFUNCTION(BlueprintCallable, Category = "HostMenu")
		void OnClickAcceptHost();

	UFUNCTION(BlueprintCallable, Category = "HostMenu")
		void OnClickToggleRightHost();

	UFUNCTION(BlueprintCallable, Category = "HostMenu")
		void OnClickToggleLeftHost();

	UPROPERTY(BlueprintReadWrite, Category = "HostMenu")
	FText PlayMode;

	UPROPERTY(BlueprintReadWrite, Category = "HostMenu")
	bool accept;

private:

	FName ServerName;

	bool lan;
};
