// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthGenerator.h"

// Sets default values
ALabyrinthGenerator::ALabyrinthGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    cells = std::vector<ACell>();
    backtrace = std::stack<ACell*>();
	createCells();
	current = &cells[0];

}

void ALabyrinthGenerator::createCells() {
    for (int row = 0; row < height; row++) {
        for (int column = 0; column < width; column++) {
            cells.push_back(ACell(row, column));
        }
    }
}

void ALabyrinthGenerator::generateMaze() {
    while (true) {
        current->setVisited(true);
        ACell* next = findNextCell();

        if (next != NULL) {
            next->setVisited(true);
            backtrace.push(current);
            current->removeWalls(*next);
            current = next;
        }
        else if (backtrace.size() > 0) {
            current = backtrace.top();
            backtrace.pop();
        }
        else if (backtrace.size() == 0) {
            break;
        }
    }
}

ACell* ALabyrinthGenerator::findNextCell() {
    std::vector<ACell*> availableNeighbors = getAvailableNeighbors();
    if (availableNeighbors.size() > 0) {
        return availableNeighbors.at(rand() % availableNeighbors.size());
    }

    return NULL;
}

std::vector<ACell*> ALabyrinthGenerator::getAvailableNeighbors() {
    std::vector<ACell*> neighbors;

    int currentRow = current->getRow();
    int currentColumn = current->getColumn();

    int neighborIndexes[4] = {
        calculateIndex(currentRow - 1, currentColumn),
        calculateIndex(currentRow, currentColumn + 1),
        calculateIndex(currentRow + 1, currentColumn),
        calculateIndex(currentRow, currentColumn - 1),
    };

    for (int i : neighborIndexes) {
        if (i != -1 && !cells[i].isVisited()) {
            neighbors.push_back(&cells[i]);
        }
    }

    return neighbors;
}

int ALabyrinthGenerator::calculateIndex(int row, int column) {
    if (row < 0 || column < 0 || column > width - 1 || row > height - 1)
        return -1;
    else
        return column + row * width;
}




// Called when the game starts or when spawned
void ALabyrinthGenerator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALabyrinthGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

