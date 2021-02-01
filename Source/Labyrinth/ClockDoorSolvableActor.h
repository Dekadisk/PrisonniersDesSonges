// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SolvableActor.h"
#include "ClockDoorSolvableActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AClockDoorSolvableActor : public ASolvableActor
{
	GENERATED_BODY()

public:

	AClockDoorSolvableActor();
	// hr
	UPROPERTY(VisibleAnywhere)
	int32 unlockCount;
	UPROPERTY(EditAnywhere)
	int32 unlockLimit;
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isOpened;

	virtual void Unlock() override;
	virtual void Lock() override;
	virtual void Activate() override;

	// Animation
	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	virtual void OnConstruction(const FTransform& Transform) override;
	
};
