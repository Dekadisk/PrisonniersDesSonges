// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Math/RandomStream.h"
#include "LabBlock.h"
#include "PuzzleRoom.h"
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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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
	void GenerateDoorMeshes();
};
