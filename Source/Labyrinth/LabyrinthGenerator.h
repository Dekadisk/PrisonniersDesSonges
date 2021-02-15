// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Cell.h"
#include <vector>
#include <stack>
#include "LabyrinthGenerator.generated.h"

UCLASS()
class LABYRINTH_API ALabyrinthGenerator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALabyrinthGenerator();

public:
	int width = 32,
		height = 32,
		tileSize = 100;


private:
	std::vector<ACell> cells;
	std::stack<ACell*> backtrace;
	ACell* current;

	void createCells();
	int calculateIndex(int i, int j);
	std::vector<ACell*> getAvailableNeighbors();
	ACell* findNextCell();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void generateMaze();

};
