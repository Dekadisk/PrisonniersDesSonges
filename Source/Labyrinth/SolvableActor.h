#pragma once

#include "CoreMinimal.h"
#include "PuzzleEventsData.h"
#include "UsableActor.h"
#include "SolvableActor.generated.h"

UCLASS(ABSTRACT)
class LABYRINTH_API ASolvableActor : public AUsableActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASolvableActor();

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isSolved;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool hasErasedHint = false;

	UPROPERTY(VisibleAnywhere)
	TArray<class AUsableActor*> Elements;
};
