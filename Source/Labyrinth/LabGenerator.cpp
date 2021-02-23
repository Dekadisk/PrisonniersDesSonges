// Fill out your copyright notice in the Description page of Project Settings.


#include "LabGenerator.h"
#include "Tile.h" 
#include "Room.h"
#include "PuzzleRoom.h"
#include "SpawnRoom.h"


// Sets default values
ALabGenerator::ALabGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	labBlocks = std::vector<LabBlock>();
	backTrace = std::stack<LabBlock*>();
	width = 4;
	height = 16;

	bandeA = 4;
	bandeB = 10;
}

// Called when the game starts or when spawned
void ALabGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {

		InitBlocks();
		CreateMaze();
		RemoveImpasse();
		RemoveLines();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DONE MAZER"));
		CreateStartRoom();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DONE ADD START ROOM"));
		CreatePuzzlesRoom();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("DONE ADD PUZZLE ROOM"));
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
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++) {
			labBlocks.push_back(LabBlock(i,j));
			if ((j == bandeA || j == bandeB || j == bandeA+1 || j == bandeB+1) && i!=0) {
				labBlocks.back().setLocked(true);
			}
		}
	}
	current = &labBlocks[0];
}

LabBlock* ALabGenerator::GetNextBlock()
{
	std::vector<LabBlock*> availableNeighbors = GetFreeNeighbours();
	if (availableNeighbors.size() > 0) {
	    return availableNeighbors.at(seed.GetUnsignedInt() % availableNeighbors.size());
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
	    if (i != -1 && !labBlocks[i].getVisited() && !labBlocks[i].IsLocked()) {
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

void ALabGenerator::RemoveLines()
{
	for (int i = 0; i < width; ++i) {
		labBlocks[GetIndex( bandeA,i)].SetToVoid();
		labBlocks[GetIndex( bandeA+1, i)].SetWallNorth(true);
		labBlocks[GetIndex( bandeA-1, i)].SetWallSouth(true);

		labBlocks[GetIndex( bandeB, i)].SetToVoid();
		labBlocks[GetIndex( bandeB + 1, i)].SetWallNorth(true);
		labBlocks[GetIndex( bandeB - 1, i)].SetWallSouth(true);

	}
}

void ALabGenerator::Conversion2Types()
{
	for (auto& block : labBlocks) {
		typeLabBlocks.push_back(Converter(block));
	}
}

int ALabGenerator::Converter(LabBlock& block) {
	if (!block.GetWalls()[LabBlock::Direction::NORTH])
	{
		if (!block.GetWalls()[LabBlock::Direction::SOUTH])
		{
			if (!block.GetWalls()[LabBlock::Direction::EAST])
			{
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (15);
				else
					return (7);
			}
			else {
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (13);
				else
					return (5);
			}
		}
		else {
			if (!block.GetWalls()[LabBlock::Direction::EAST])
			{
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (11);
				else
					return (3);
			}
			else {
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (9);
				else
					return (1);
			}
		}
	}
	else {
		if (!block.GetWalls()[LabBlock::Direction::SOUTH])
		{
			if (!block.GetWalls()[LabBlock::Direction::EAST])
			{
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (14);
				else
					return (6);
			}
			else {
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (12);
				else
					return (4);
			}
		}
		else {
			if (!block.GetWalls()[LabBlock::Direction::EAST])
			{
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (10);
				else
					return (2);
			}
			else {
				if (!block.GetWalls()[LabBlock::Direction::WEST])
					return (8);
				else
					return (0);
			}
		}
	}
}

void ALabGenerator::RemoveImpasse() {
	for (int i = 0; i < labBlocks.size(); i++) {
		if (Converter(labBlocks[i]) == 1 && labBlocks[i].GetY() != height-1) { // ajouter des conditions pour ne pas modifier tous ceux sur les bords
			
			labBlocks[i].SetWallSouth(false);
			labBlocks[i + 1].SetWallNorth(false);

		} else if (Converter(labBlocks[i]) == 2 && labBlocks[i].GetX() != 0) {
			labBlocks[i].SetWallWest(false);
			labBlocks[i - height].SetWallEast(false);

		}
		else if (Converter(labBlocks[i]) == 4 && labBlocks[i].GetY() != 0 ) {
			labBlocks[i].SetWallNorth(false);
			labBlocks[i - 1].SetWallSouth(false);
		}
		else if (Converter(labBlocks[i]) == 8 && labBlocks[i].GetX() != width-1) {
			labBlocks[i].SetWallEast(false);
			labBlocks[i + height].SetWallWest(false);
		}
	}
}

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

void ALabGenerator::CreateStartRoom()
{
	float ratio = 2.f;
	int randomCol = seed.GetUnsignedInt() % width;
	labBlocks[GetIndex(0, randomCol)].SetWallNorth(false);
	ASpawnRoom* spawnRoom = GetWorld()->SpawnActor<ASpawnRoom>(ASpawnRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -(randomCol) * 550.f * ratio ,550.f * ratio,0 }, FVector{ ratio,ratio,ratio }));
}

void ALabGenerator::CreatePuzzlesRoom()
{
	float ratio = 2.f;
	int randomCol1 = seed.GetUnsignedInt() % width;
	int randomCol2 = seed.GetUnsignedInt() % width;
	int randomCol3 = seed.GetUnsignedInt() % width;

	labBlocks[GetIndex(bandeA - 1, randomCol1)].SetWallSouth(false);
	labBlocks[GetIndex(bandeA + 2, randomCol1)].SetWallNorth(false);
	labBlocks[GetIndex(bandeB - 1, randomCol2)].SetWallSouth(false);
	labBlocks[GetIndex(bandeB + 2, randomCol2)].SetWallNorth(false);
	labBlocks[GetIndex(height - 1, randomCol3)].SetWallSouth(false);
	APuzzleRoom* puzzleRoom1 = GetWorld()->SpawnActor<APuzzleRoom>(APuzzleRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -randomCol1 * 550.f * ratio , -550.f * ratio * bandeA, 0 }, FVector{ ratio,ratio,ratio }));
	APuzzleRoom* puzzleRoom2 = GetWorld()->SpawnActor<APuzzleRoom>(APuzzleRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -randomCol2 * 550.f * ratio , -550.f * ratio * bandeB, 0 }, FVector{ ratio,ratio,ratio }));
	APuzzleRoom* puzzleRoom3 = GetWorld()->SpawnActor<APuzzleRoom>(APuzzleRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -randomCol3 * 550.f * ratio , -550.f * ratio * height, 0}, FVector{ ratio,ratio,ratio }));
}




