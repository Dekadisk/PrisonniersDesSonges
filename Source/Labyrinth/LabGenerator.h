// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabBlock.h"
#include <vector>
#include <stack>
#include "LabGenerator.generated.h"

UCLASS()
class LABYRINTH_API ALabGenerator : public AActor
{
	GENERATED_BODY()

private:
	int width{ 3 }, height{ 3 };

	std::vector<LabBlock> labBlocks;
	std::stack<LabBlock*> backTrace;
	std::vector<int> typeLabBlocks;
	LabBlock* current;

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

	void InitBlocks();
	int GetIndex(int i, int j);
	std::vector<LabBlock*> GetFreeNeighbours();
	LabBlock* GetNextBlock();
	void CreateMaze();
	void Conversion2Types();
	void GenerateMazeMesh();
};
