#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"
#include "LabBlock.h"
#include "ClockPuzzleRoom.h"
#include "SpawnRoom.h"
#include <vector>
#include <stack>
#include "LabGenerator.generated.h"

class ASpawnRoom;
class ATile;

UCLASS()
class LABYRINTH_API ALabGenerator : public AActor
{
	GENERATED_BODY()

private:
	ASpawnRoom* spawnRoom;
	LabBlock* current;
	int height;

	std::stack<LabBlock*> backTrace;
	
	std::vector<int> typeLabBlocks;
	std::vector<int> bandes;
	
	std::vector<LabBlock> labBlocks;

	UPROPERTY()
	TArray<APuzzleRoom*> puzzleRooms;

	UPROPERTY()
	TArray<ATile*> tiles;

	std::vector<LabBlock*> doors;

	std::vector<LabBlock*> keys;

	std::vector<LabBlock*> tilesBeginSection;

	std::vector<LabBlock*> hintClockPos;

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
	/*void CreateStartRoom();*/
	void CreatePuzzlesRoom();
	void GenerateMazeMesh();
	void DrawDebugLabGraph();
	AActor* InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation = FRotator::ZeroRotator, FVector scale = {1.f,1.f,1.f});
	void GenerateDoorMeshes();
	void GenerateKeyMeshes();
	void GenerateHintMeshes();
	void GenerateTargetPoint();
	void InitKeys();
	void InitHints();
	void SpawnNavMesh();
};
