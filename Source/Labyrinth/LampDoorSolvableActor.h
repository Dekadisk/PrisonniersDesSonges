// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SolvableActor.h"
#include "LampDoorSolvableActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALampDoorSolvableActor : public ASolvableActor
{
	GENERATED_BODY()
public:

	UPROPERTY(VisibleAnywhere)
		int32 unlockCount;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", UIMin = "1"))
		int32 unlockLimit;


	virtual void Unlock(bool Event) override;
	virtual void Lock(bool Event) override;
	virtual void Activate(bool Event) override;
	virtual void On(bool Event) override;
	virtual void Off(bool Event) override;

	// Animation
	UFUNCTION(BlueprintImplementableEvent)
		void Animate();

	virtual void OnConstruction(const FTransform& Transform) override;

	int GetEtat() override;

};
