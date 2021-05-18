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

	UPROPERTY(EditAnywhere, Category = "Etat", ReplicatedUsing = OnRep_OpenCloseDoor)
	bool bIsOpen;

	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	//Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void OpenAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void CloseAnimation();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus();

	void OnEndFocus();

	UFUNCTION()
	void OnRep_OpenCloseDoor();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:

	float fOpenPercent;
	float fSpeedOpen;
};
