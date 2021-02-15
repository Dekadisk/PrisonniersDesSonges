// Fill out your copyright notice in the Description page of Project Settings.


#include "Cell.h"

// Sets default values
ACell::ACell()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

ACell::ACell(int row, int column) 
{

	PrimaryActorTick.bCanEverTick = true;

	this->row = row;
	this->column = column;
	visited = false;
	setWalls();
}

void ACell::setWalls() 
{
	for (int i = 0; i < 4; i++) walls[i] = true;
}

void ACell::setVisited(bool b) { visited = b; }
bool ACell::isVisited() { return visited; }

int ACell::getRow() { return row; }
int ACell::getColumn() { return column; }

void ACell::removeWalls(ACell& next) {
	int x = column - next.column;
	int y = row - next.row;

	if (x == 1) {
		walls[LEFT] = false;
		next.walls[RIGHT] = false;
	}
	else if (x == -1) {
		walls[RIGHT] = false;
		next.walls[LEFT] = false;
	}
	else if (y == 1) {
		walls[TOP] = false;
		next.walls[BOTTOM] = false;
	}
	else if (y == -1) {
		walls[BOTTOM] = false;
		next.walls[TOP] = false;
	}
}

// Called when the game starts or when spawned
void ACell::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ACell::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

