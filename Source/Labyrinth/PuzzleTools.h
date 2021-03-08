#pragma once
#include "PuzzleTools.generated.h"

class ASolvableActor;
class APuzzleActor;

UENUM()
enum class EPuzzleActions : uint8 {
	None,
	Unlock,
	Lock,
	Activate,
	Ring,
	Open,
	Close
};

UENUM()
enum class EPasswords : uint8 {
	Hope,
	Pain,
	Fear,
	Soul
};

UENUM()
enum class EPlateLetter : uint8 {
	None,
	A,
	B,
	C,
	D,
	E,
	F,
	G,
	H,
	I,
	J,
	K,
	L,
	M,
	N,
	O,
	P,
	Q,
	R,
	S,
	T,
	U,
	V,
	W,
	X,
	Y,
	Z
};

USTRUCT(BlueprintType)
struct FLinkedActors
{
	GENERATED_BODY()
	UPROPERTY(EditAnywhere)
	ASolvableActor * linkedActor;

	UPROPERTY(EditAnywhere)
	EPuzzleActions yes;

	UPROPERTY(EditAnywhere)
	EPuzzleActions no;
};

USTRUCT(BlueprintType)
struct FPuzzleElemsNeeded
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<APuzzleActor> type;

	UPROPERTY(EditAnywhere)
	uint8 number;
};

void ExecuteAction(APuzzleActor* instigator, ASolvableActor* target, EPuzzleActions action);