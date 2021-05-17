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

	ABellDoorSolvableActor* stoneDoorActor;

	void InitPuzzle(FRandomStream seed, PuzzleDifficulty difficulty) override;

	//UFUNCTION()
	void CreateBells(std::vector<LabBlock*> bells,LabBlock* bellHintPos, const TArray<ATile*>& tiles);

	//UFUNCTION()
	AActor* InstanceBell(const TCHAR* bpName, FVector location, FRotator rotation = FRotator::ZeroRotator, FVector scale = { 1.f,1.f,1.f });
};
