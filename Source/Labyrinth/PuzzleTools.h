#pragma once
#include "SolvableActor.h"

#include "PuzzleTools.generated.h"

UENUM()
enum class EPuzzleActions : uint8 {
	None,
	Unlock,
	Lock,
	Activate
};

USTRUCT(BlueprintType)
struct FLinkedActors
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	ASolvableActor * linkedActor;

	UPROPERTY(EditDefaultsOnly)
	EPuzzleActions yes;

	UPROPERTY(EditDefaultsOnly)
	EPuzzleActions no;
};

void ExecuteAction(ASolvableActor* target, EPuzzleActions action);