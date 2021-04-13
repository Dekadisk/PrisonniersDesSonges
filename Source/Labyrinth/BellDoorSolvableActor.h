#pragma once

#include "CoreMinimal.h"
#include "SolvableActor.h"
#include "BellHintActor.h"
#include "BellDoorSolvableActor.generated.h"

UCLASS()
class LABYRINTH_API ABellDoorSolvableActor : public ASolvableActor
{
	GENERATED_BODY()
	
public:
	ABellDoorSolvableActor();

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	TArray<int32> waited;

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	TArray<int32> playing;

	ABellHintActor* hint;

	virtual void Ring(bool Event, int32 note) override;

	// Animation
	UFUNCTION(BlueprintImplementableEvent)
	void Animate();
};
