#pragma once

#include "CoreMinimal.h"
#include "SolvableActor.h"
#include "ClockDoorSolvableActor.generated.h"

UCLASS()
class LABYRINTH_API AClockDoorSolvableActor : public ASolvableActor
{
	GENERATED_BODY()

public:

	AClockDoorSolvableActor();

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
	
};
