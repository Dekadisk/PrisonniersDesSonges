#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"
#include "LabBlock.h"
#include "SpawnRoom.h"
#include <vector>
#include <stack>
#include "PuzzleRoom.h"
#include "DebugMesh.h"
#include "LabGenerator.generated.h"

class ATile;

UCLASS()
class LABYRINTH_API ALabGenerator : public AActor
{
	GENERATED_BODY()

private:

	enum PuzzleType {
		Clock,
		Bell,
		Lamp
	};
	UPROPERTY()
	ASpawnRoom* spawnRoom;
	LabBlock* current;
	int height;

	std::stack<LabBlock*> backTrace;
	
	std::vector<int> typeLabBlocks;
	std::vector<int> bandes;
	
	std::vector<LabBlock> labBlocks;

	UPROPERTY()
	TArray<APuzzleRoom*> puzzleRooms;

	std::vector<PuzzleType> puzzleRoomsType;

	UPROPERTY()
	TArray<ATile*> tiles;

	UPROPERTY()
	TArray<AActor*> Starts;

	std::vector<LabBlock*> doors;

	std::vector<LabBlock*> keys;
	std::vector<LabBlock*> hidingSpots;

	std::vector<LabBlock*> tilesBeginSection;

	std::vector<std::vector<LabBlock*>> hintClockPos;
	std::vector<std::vector<LabBlock*>> bellPos;
	std::vector<std::vector<LabBlock*>> clockPos;
	std::vector<std::vector<LabBlock*>> lampPos;
	std::vector<std::vector<LabBlock*>> bellHintPos;
	
	//DEBUG
	TArray<ADebugMesh*> debugMeshInfMap;

	UPROPERTY()
	TSubclassOf<ASpawnRoom> SpawnRoom_BP;
public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<int> nbSubSections;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int subSectionSize;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int width;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Random")
	FRandomStream seed;

public:	

	// Sets default values for this actor's properties
	ALabGenerator();

	virtual void Tick(float some_float) override;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	UFUNCTION()
	int GetHeight() { return height; };
	UFUNCTION()
	int GetWidth() { return width; };

	UFUNCTION()
	void InitSize();
	UFUNCTION()
	void InitBlocks();
	UFUNCTION()
	int GetIndex(int x, int y);
	std::vector<LabBlock*> GetFreeNeighbours();
	LabBlock* GetNextBlock();
	UFUNCTION()
	void CreateMaze();
	UFUNCTION()
	void RemoveLines();
	UFUNCTION()
	void Conversion2Types();
	int Converter(LabBlock& labblock);
	UFUNCTION()
	void RemoveImpasse();
	UFUNCTION()
	void CreateStartRoom();
	UFUNCTION()
	void CreatePuzzlesRoom();
	UFUNCTION()
	void GenerateMazeMesh();
	UFUNCTION()
	void DrawDebugLabGraph();
	UFUNCTION()
	void DrawDebugInfluenceMap();
	UFUNCTION()
	AActor* InstanceBP(TSubclassOf<UObject> blueprint, FVector location, FRotator rotation, FVector scale);
	UFUNCTION()
	void GenerateDoorMeshes();
	UFUNCTION()
	void GenerateObjectsMeshes();
	UFUNCTION()
	void GenerateHintMeshes();
	UFUNCTION()
	void GenerateTargetPoint();
	UFUNCTION()
	void GeneratePuzzleObjectsMeshes();
	UFUNCTION()
	void GenerateDecorationMeshes();
	UFUNCTION()
	void InitObjects();
	UFUNCTION()
	void InitPuzzleObjects();
	UFUNCTION()
	void SpawnNavMesh();
	UFUNCTION()
	void SpawnMonster();
	UFUNCTION()
	void UpdateInfluenceMap();
	UFUNCTION()
	void PropagateInfluenceMap();
	UFUNCTION()
	float GetCellInfluenceAtPos(FVector absPos);
};
