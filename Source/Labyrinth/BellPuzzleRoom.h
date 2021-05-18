#pragma once

#include "CoreMinimal.h"
#include "PuzzleRoom.h"
#include "LabBlock.h"
#include "Tile.h"
#include "BellDoorSolvableActor.h"
#include <vector>
#include "BellPuzzleActor.h"
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

	//BLUEPRINTS
	UPROPERTY()
	TSubclassOf<ABellDoorSolvableActor> BellDoorSolvableActor_BP;
	UPROPERTY()
	TSubclassOf<ABellHintActor> BellHintActor_BP;
	UPROPERTY()
	TSubclassOf<ABellPuzzleActor> BellPuzzleActor_BP;

	ABellDoorSolvableActor* stoneDoorActor;

	void InitPuzzle(FRandomStream seed, PuzzleDifficulty difficulty) override;

	//UFUNCTION()
	void CreateBells(std::vector<LabBlock*> bells,LabBlock* bellHintPos, const TArray<ATile*>& tiles);

	//UFUNCTION()
	AActor* InstanceBell( FVector location, FRotator rotation = FRotator::ZeroRotator, FVector scale = { 1.f,1.f,1.f });
};
