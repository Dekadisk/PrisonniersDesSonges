// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Cell.generated.h"

UCLASS()
class LABYRINTH_API ACell : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACell();

	ACell(int i, int j);

private:

	int row, column;
	bool walls[4];
	bool visited;
	enum direction { TOP, RIGHT, BOTTOM, LEFT };

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:

	ACell* checkNeighbors();

	void removeWalls(ACell&);

	void setWalls();
	void setVisited(bool);
	bool isVisited();

	int getRow();
	int getColumn();

};
