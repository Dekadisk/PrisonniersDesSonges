#pragma once
#include <vector>
#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include "LabBlock.h"
#include "Tile.h"
#include "ButtonPuzzleActor.h"
#include "ClockDoorSolvableActor.h"
#include "ClockPuzzleActor.h"
#include "LeverPuzzleActor.h"
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
	AButtonPuzzleActor* leverActor;

public:

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	int nbClocks;
	
	UPROPERTY()
	TArray<HandDir> solutions;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Random")
	FRandomStream randomSeed;

	//BLUEPRINT
	UPROPERTY()
	TSubclassOf<AButtonPuzzleActor> ButtonLeverPuzzleActor_BP;
	UPROPERTY()
	TSubclassOf<AClockDoorSolvableActor> ClockDoorSolvableActor_BP;
	UPROPERTY()
	TSubclassOf<AClockPuzzleActor> ClockPuzzleActor_BP;

	void InitPuzzle(FRandomStream seed, PuzzleDifficulty difficulty)override;

	void CreateClocks(std::vector<LabBlock*> clocksPos, const TArray<ATile*>& tiles);
	
};
