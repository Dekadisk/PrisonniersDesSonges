// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include <vector>
#include "LabBlock.h"
#include "Tile.h"
#include "LampDoorSolvableActor.h"
#include "LampPuzzleActor.h"
#include "LampPuzzleRoom.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALampPuzzleRoom : public APuzzleRoom
{
	GENERATED_BODY()

public:

	ALampPuzzleRoom();

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Random")
		FRandomStream randomSeed;

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	int nbLampsPerGroup; 
	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	int nbGroups;
	UPROPERTY()
	TArray<ALampPuzzleActor*> lampsActors;

	ALampDoorSolvableActor* solvableActor;

	//BLUEPRINTS
	UPROPERTY()
	TSubclassOf<ALampPuzzleActor> LampPuzzleActor_BP;
	UPROPERTY()
	TSubclassOf<ALampDoorSolvableActor> LampDoorSolvableActor_BP;
	void InitPuzzle(FRandomStream seed, PuzzleDifficulty difficulty) override;
	void CreateLamps(std::vector<LabBlock*> lamps, const TArray<ATile*>& tiles);
};
