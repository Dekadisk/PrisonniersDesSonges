#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include "BellPuzzleRoom.generated.h"

UCLASS()
class LABYRINTH_API ABellPuzzleRoom : public APuzzleRoom
{
	GENERATED_BODY()

public:
	ABellPuzzleRoom();
public:

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	int nbBells;
};
