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

	int GetHeight() { return height; };
	int GetWidth() { return width; };

	void InitSize();
	void InitBlocks();
	int GetIndex(int x, int y);
	std::vector<LabBlock*> GetFreeNeighbours();
	LabBlock* GetNextBlock();
	void CreateMaze();
	void RemoveLines();
	void Conversion2Types();
	int Converter(LabBlock& labblock);
	void RemoveImpasse();
	void CreateStartRoom();
	void CreatePuzzlesRoom();
	void GenerateMazeMesh();
	void DrawDebugLabGraph();
	void DrawDebugInfluenceMap();
	AActor* InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation = FRotator::ZeroRotator, FVector scale = {1.f,1.f,1.f});
	void GenerateDoorMeshes();
	void GenerateObjectsMeshes();
	void GenerateHintMeshes();
	void GenerateTargetPoint();
	void GeneratePuzzleObjectsMeshes();
	void GenerateDecorationMeshes();
	void InitObjects();
	void InitPuzzleObjects();
	void SpawnNavMesh();
	void SpawnMonster();
	void UpdateInfluenceMap();
	void PropagateInfluenceMap();
};
