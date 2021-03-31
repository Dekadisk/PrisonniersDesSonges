#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "DoorActor.generated.h"

UCLASS()
class LABYRINTH_API ADoorActor : public AUsableActor
{
	GENERATED_BODY()

public:
	ADoorActor();

public:
	UPROPERTY(EditAnywhere, Category = "Etat", Replicated)
	bool bIsLocked;
	UPROPERTY(EditAnywhere, Category = "Etat", Replicated)
	bool bIsOpen;

	virtual void OnUsed(AActor* InstigatorActor) override;

	//Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void OpenAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void CloseAnimation();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:
	float fOpenPercent;
	float fSpeedOpen;
};
