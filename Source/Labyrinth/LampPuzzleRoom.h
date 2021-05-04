// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include <vector>
#include "LabBlock.h"
#include "Tile.h"
#include "LampDoorSolvableActor.h"
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

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
		int nbLamps;

	ALampDoorSolvableActor* stoneDoorActor;

	void InitPuzzle(FRandomStream seed) override;
	void CreateLamps(std::vector<LabBlock*> lamps, const TArray<ATile*>& tiles);
};
