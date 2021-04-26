#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include <vector>
#include "LabBlock.h"
#include "Tile.h"
#include "LeverPuzzleActor.h"
#include "ClockDoorSolvableActor.h"
#include "ClockPuzzleRoom.generated.h"

UENUM()
enum class HandDir {
	UP,
	UP_RIGHT,
	RIGHT,
	DOWN_RIGHT,
	DOWN,
	DOWN_LEFT,
	LEFT,
	UP_LEFT
};

UCLASS()
class LABYRINTH_API AClockPuzzleRoom : public APuzzleRoom
{
	GENERATED_BODY()

public:
	AClockPuzzleRoom();

	AClockDoorSolvableActor* herseActor;
	ALeverPuzzleActor* leverActor;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	int nbClocks;
	
	UPROPERTY()
	TArray<HandDir> solutions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Random")
	FRandomStream randomSeed;

	void InitPuzzle(FRandomStream seed) override;

	void CreateClocks(std::vector<LabBlock*> clocksPos, const TArray<ATile*>& tiles);
	
};
