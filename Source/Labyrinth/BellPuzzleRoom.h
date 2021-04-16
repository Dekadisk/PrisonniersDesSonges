#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include "LabBlock.h"
#include "Tile.h"
#include "BellDoorSolvableActor.h"
#include <vector>
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

	UPROPERTY(EditDefaultsOnly, Category = "Puzzle")
	TArray<int32> sons;

	UPROPERTY()
	ABellDoorSolvableActor* stoneDoorActor;

	void InitPuzzle(FRandomStream seed) override;
	void CreateBells(std::vector<LabBlock*> bells, const TArray<ATile*>& tiles);
	AActor* InstanceBell(const TCHAR* bpName, FVector location, FRotator rotation = FRotator::ZeroRotator, FVector scale = { 1.f,1.f,1.f });
};
