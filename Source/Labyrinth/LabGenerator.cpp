// Fill out your copyright notice in the Description page of Project Settings.


#include "LabGenerator.h"
#include "Tile.h" 

// Sets default values
ALabGenerator::ALabGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	labBlocks = std::vector<LabBlock>();
	backTrace = std::stack<LabBlock*>();

	InitBlocks();
	current = &labBlocks[0];
}

// Called when the game starts or when spawned
void ALabGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		CreateMaze();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DONE MAZER"));
		Conversion2Types();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DONE TYPER"));
		GenerateMazeMesh();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DONE GENERATE MESHES"));
	}
}

// Called every frame
void ALabGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALabGenerator::InitBlocks() {
	for (int i = 0; i < height; i++)
	{
		for (int j = 0; j < width; j++) {
			labBlocks.push_back(LabBlock(i,j));
		}
	}
}

LabBlock* ALabGenerator::GetNextBlock()
{
	std::vector<LabBlock*> availableNeighbors = GetFreeNeighbours();
	if (availableNeighbors.size() > 0) {
	    return availableNeighbors.at(rand() % availableNeighbors.size());
	}
	
	return NULL;
}

std::vector<LabBlock*> ALabGenerator::GetFreeNeighbours() {
	std::vector<LabBlock*> neighbors;
	
	int currentRow = current->GetY();
	int currentColumn = current->GetX();
	
	int neighborIndexes[4] = {
	    GetIndex(currentRow - 1, currentColumn),
		GetIndex(currentRow + 1, currentColumn),
	    GetIndex(currentRow, currentColumn + 1),
	    GetIndex(currentRow, currentColumn - 1)
	};
	
	for (int i : neighborIndexes) {
	    if (i != -1 && !labBlocks[i].getVisited()) {
	        neighbors.push_back(&labBlocks[i]);
	    }
	}
	
	return neighbors;
}
	
int ALabGenerator::GetIndex(int row, int column) {
	if (row < 0 || column < 0 || column > width - 1 || row > height - 1)
	    return -1;
	else
	    return column*height + row;
}

void ALabGenerator::CreateMaze() {
	while (true) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Boucle"));
		current->setVisited(true);
		LabBlock* next = GetNextBlock();
		
		if (next != NULL) {
		    next->setVisited(true);
		    backTrace.push(current);
		    current->removeWalls(*next);
		    current = next;
		}
		else if (backTrace.size() > 0) {
		    current = backTrace.top();
		    backTrace.pop();
		}
		else if (backTrace.size() == 0) {
		    break;
		}
	}
}

void ALabGenerator::Conversion2Types()
{
	for (auto& block : labBlocks) {
		if (!block.GetWalls()[LabBlock::Direction::NORTH])
		{
			if (!block.GetWalls()[LabBlock::Direction::SOUTH])
			{
				if (!block.GetWalls()[LabBlock::Direction::EAST])
				{
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(15);
					else
						typeLabBlocks.push_back(7);
				}
				else {
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(13);
					else
						typeLabBlocks.push_back(5);
				}
			}
			else {
				if (!block.GetWalls()[LabBlock::Direction::EAST])
				{
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(11);
					else
						typeLabBlocks.push_back(3);
				}
				else {
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(9);
					else
						typeLabBlocks.push_back(1);
				}
			}
		}
		else {
			if (!block.GetWalls()[LabBlock::Direction::SOUTH])
			{
				if (!block.GetWalls()[LabBlock::Direction::EAST])
				{
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(14);
					else
						typeLabBlocks.push_back(6);
				}
				else {
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(12);
					else
						typeLabBlocks.push_back(4);
				}
			}
			else {
				if (!block.GetWalls()[LabBlock::Direction::EAST])
				{
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(10);
					else
						typeLabBlocks.push_back(2);
				}
				else {
					if (!block.GetWalls()[LabBlock::Direction::WEST])
						typeLabBlocks.push_back(8);
					else
						typeLabBlocks.push_back(0);
				}
			}
		}

	}
}
//o
void ALabGenerator::GenerateMazeMesh()
{
	float ratio = 2.f;
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			ATile* tileIteration = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), FTransform(FQuat::Identity, FVector{ -i * 550.f * ratio ,-j * 550.f * ratio,0 }, FVector{ ratio,ratio,ratio }));
			tileIteration->kind = typeLabBlocks[i * height + j];
			tileIteration->UpdateMesh();
		}
	}
}


