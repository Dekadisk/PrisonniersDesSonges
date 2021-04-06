#include "LabGenerator.h"
#include "Tile.h" 
#include "Room.h"
#include "ClockPuzzleRoom.h"
#include "SpawnRoom.h"
#include "DrawDebugHelpers.h"
#include <algorithm>
#include "UsableActor.h"
#include "HintDecalActor.h"
#include "LabyrinthNavMesh.h"
#include "Engine/DecalActor.h"
#include "Engine/StaticMeshSocket.h"
#include "NavigationSystem.h"
#include "Components/DecalComponent.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>

// Sets default values
ALabGenerator::ALabGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	labBlocks = std::vector<LabBlock>();
	backTrace = std::stack<LabBlock*>();
	width = 4;
	height = -1;
	nbSubSections = { 2,3,4 };
	subSectionSize = 6;
}

// Called when the game starts or when spawned
void ALabGenerator::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority()) {
		
		InitSize();
		InitBlocks();
		CreateMaze();
		RemoveImpasse();
		RemoveLines();
		CreateStartRoom();
		CreatePuzzlesRoom();
		InitKeys();
		InitHints();
		Conversion2Types();
		GenerateMazeMesh();
		GenerateDoorMeshes();
		GenerateKeyMeshes();
		GenerateHintMeshes();
		SpawnNavMesh();
	}
	//DEBUG
	DrawDebugLabGraph();
}

void ALabGenerator::InitSize() {
	int totalNbSubSection = 0;
	for (int nbSubSectionsOfSection : nbSubSections)
		totalNbSubSection += nbSubSectionsOfSection;
	height = totalNbSubSection * subSectionSize + 2 * (nbSubSections.Num() - 1);

	for (int i = 0; i < nbSubSections.Num() - 1; ++i) {
		if (i == 0)
			bandes.push_back(nbSubSections[i] * subSectionSize);
		else
			bandes.push_back(nbSubSections[i] * subSectionSize + bandes[i - 1] + 2);
	}
}

void ALabGenerator::SpawnNavMesh() {

	int Sum{};
	for (int i = 0; i < nbSubSections.Num(); i++) {
		Sum += nbSubSections[i];
	}
	FVector positionCentre = FVector{ -225.f * width, -0.5f*(nbSubSections.Num()*1100+Sum*550*subSectionSize), 0.f};

	AActor* NavMesh = UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass());
	NavMesh->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	NavMesh->SetActorLocation(positionCentre, false);
	NavMesh->SetActorRelativeScale3D(FVector{ width+1.f, 1.f+(nbSubSections.Num() * 2.f + Sum * subSectionSize), 1.0f });
	NavMesh->GetRootComponent()->UpdateBounds();
	UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	NavigationArea->UNavigationSystemV1::OnNavigationBoundsUpdated(Cast<ANavMeshBoundsVolume>(NavMesh));

}

void ALabGenerator::InitBlocks() {
	
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++) {
			labBlocks.push_back(LabBlock(i,j,GetIndex(i,j)));
			if ((std::find(begin(bandes), end(bandes), j) != end(bandes)
				|| std::find(begin(bandes), end(bandes), j - 1) != end(bandes)) && i != 0) {
				labBlocks.back().setLocked(true);
			}
		}
	}
	current = &labBlocks[0];
	// Division des Sections //
	int globalSectionHeight = 0;
	for (int section = 0; section < nbSubSections.Num(); ++section) {
		for (int subSection = 0; subSection < nbSubSections[section] - 1; ++subSection) {
			int h1 = floor((subSection + 0.5f) * subSectionSize)+ globalSectionHeight;
			int h2 = floor((subSection + 1 + 0.5f) * subSectionSize) + globalSectionHeight;
			int currentWall = seed.GetUnsignedInt() % (h2 - h1);
			int randDoor = seed.GetUnsignedInt() % width;
			if (randDoor != 0)
				labBlocks[GetIndex(0, currentWall + h1)].setLocked(true);
			else {
				labBlocks[GetIndex(0, currentWall + h1)].SetHasDoor(true);
				doors.push_back(&labBlocks[GetIndex(0, currentWall + h1)]);
				//lock door adjacent tiles South
				if (randDoor < width - 1)
					labBlocks[GetIndex(randDoor + 1,currentWall + h1)].setLocked(true);
			}
			for (int i = 1; i < width; ++i) {
				int rand = seed.GetUnsignedInt() % (h2 - h1);
				if (rand < currentWall)
					currentWall -= 1;
				else if (rand > currentWall)
					currentWall += 1;
				if (i != randDoor)
					labBlocks[GetIndex(i,currentWall + h1)].setLocked(true);
				else {
					labBlocks[GetIndex(i,currentWall + h1)].SetHasDoor(true);
					doors.push_back(&labBlocks[GetIndex(i, currentWall + h1)]);
					//lock door adjacent tiles North South
					labBlocks[GetIndex(randDoor - 1, currentWall + h1)].setLocked(true);
					if (randDoor < width - 1)
						labBlocks[GetIndex(randDoor + 1, currentWall + h1)].setLocked(true);
				}
			}
		}
		globalSectionHeight += nbSubSections[section] * subSectionSize+2;
	} 
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
	    GetIndex(currentColumn,currentRow - 1),
		GetIndex(currentColumn,currentRow + 1),
	    GetIndex(currentColumn + 1,currentRow),
	    GetIndex(currentColumn - 1,currentRow)
	};
	
	for (int i : neighborIndexes) {
	    if (i != -1 && !labBlocks[i].getVisited() && !labBlocks[i].IsLocked()) {
	        neighbors.push_back(&labBlocks[i]);
	    }
	}
	
	return neighbors;
}
	
int ALabGenerator::GetIndex(int column, int row) {
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
		std::for_each(bandes.begin(), bandes.end(),
			[&](int bande) {
				labBlocks[GetIndex(i,bande)].SetWallsToVoid();
				labBlocks[GetIndex(i,bande)].SetNeighborsToVoid();
				labBlocks[GetIndex(i,bande)].setLocked(true);

				labBlocks[GetIndex(i,bande + 1)].SetWallsToVoid();
				labBlocks[GetIndex(i, bande + 1)].SetNeighborsToVoid();
				labBlocks[GetIndex(i, bande + 1)].setLocked(true);

				labBlocks[GetIndex(i, bande + 2)].SetWallNorth(true);
				labBlocks[GetIndex(i, bande + 2)].SetNeighborNorth(nullptr);

				labBlocks[GetIndex(i, bande - 1)].SetWallSouth(true);
				labBlocks[GetIndex(i, bande - 1)].SetNeighborSouth(nullptr);
			});
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
//
void ALabGenerator::RemoveImpasse() {
	for (int i = 0; i < labBlocks.size(); i++) {
		if (labBlocks[i].IsLocked())
			continue;
		else if (Converter(labBlocks[i]) == 1 && labBlocks[i].GetY() != height-1 && !labBlocks[i + 1].IsLocked()) {
			
			labBlocks[i].SetWallSouth(false);
			labBlocks[i].SetNeighborSouth(&labBlocks[i + 1]);
			labBlocks[i + 1].SetWallNorth(false);
			labBlocks[i+1].SetNeighborNorth(&labBlocks[i]);

		} else if (Converter(labBlocks[i]) == 2 && labBlocks[i].GetX() != 0 && !labBlocks[i - height].IsLocked()) {
			labBlocks[i].SetWallWest(false);
			labBlocks[i].SetNeighborWest(&labBlocks[i - height]);
			labBlocks[i - height].SetWallEast(false);
			labBlocks[i - height].SetNeighborEast(&labBlocks[i]);

		}
		else if (Converter(labBlocks[i]) == 4 && labBlocks[i].GetY() != 0 && !labBlocks[i - 1].IsLocked()) {
			labBlocks[i].SetWallNorth(false);
			labBlocks[i].SetNeighborNorth(&labBlocks[i - 1]);
			labBlocks[i - 1].SetWallSouth(false);
			labBlocks[i - 1].SetNeighborSouth(&labBlocks[i]);
		}
		else if (Converter(labBlocks[i]) == 8 && labBlocks[i].GetX() != width-1 && !labBlocks[i + height].IsLocked()) {
			labBlocks[i].SetWallEast(false);
			labBlocks[i].SetNeighborEast(&labBlocks[i + height]);
			labBlocks[i + height].SetWallWest(false);
			labBlocks[i + height].SetNeighborWest(&labBlocks[i]);
		}
	}
}

void ALabGenerator::GenerateMazeMesh()
{
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			ATile* tileIteration = GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), FTransform(FQuat::Identity, FVector{ -i * LabBlock::assetSize ,-j * LabBlock::assetSize,0 }, FVector{ 1.f,1.f,1.f }));
			tileIteration->kind = typeLabBlocks[i * height + j];
			tileIteration->UpdateMesh();
			tileIteration->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			tiles.Add(tileIteration);
		}
	}
}

void ALabGenerator::DrawDebugLabGraph()
{
	for (LabBlock& labBlock : labBlocks) {
		if (labBlock.IsLocked())
			continue;
		if (labBlock.GetHasKey()) {
			DrawDebugSphere(GetWorld(), labBlock.GetGlobalPos(), 30, 4, FColor(255, 255, 0), true);
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetGlobalPos() + FVector{ 0,0,300 }, FColor(255, 255, 0), true);
			DrawDebugString(GetWorld(), labBlock.GetGlobalPos(), "KEY");
		}
		if (labBlock.GetHasHint()) {
			DrawDebugSphere(GetWorld(), labBlock.GetGlobalPos(), 30, 4, FColor(255, 0, 255), true);
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetGlobalPos() + FVector{0,0,300}, FColor(255, 0, 255), true);
			DrawDebugString(GetWorld(), labBlock.GetGlobalPos(), "HINT");
		}
		if (labBlock.GetHasDoor())
			DrawDebugBox(GetWorld(), labBlock.GetGlobalPos(), { LabBlock::assetSize / 2,LabBlock::assetSize / 6,LabBlock::assetSize / 2 }, FColor(0, 255, 0), true);
		DrawDebugBox(GetWorld(), labBlock.GetGlobalPos(), { LabBlock::assetSize /2,LabBlock::assetSize / 2,0}, FColor(255, 0, 0), true);
		if (labBlock.GetNeighborNorth() != nullptr)
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetNeighborNorth()->GetGlobalPos(), FColor::Blue,true);
		if (labBlock.GetNeighborWest() != nullptr)
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetNeighborWest()->GetGlobalPos(), FColor::Blue, true);
	}
	for (APuzzleRoom* puzzleRoom : puzzleRooms)
	{
		DrawDebugBox(GetWorld(), puzzleRoom->GetTransform().GetLocation() + 
			FVector{0.5f*(puzzleRoom->width-1) * LabBlock::assetSize ,-0.5f * (puzzleRoom->height - 1) * LabBlock::assetSize,0},
			{ puzzleRoom->width*LabBlock::assetSize / 2,puzzleRoom->height * LabBlock::assetSize  / 2,1 }, FColor(0, 255, 0), true);
	}
}

AActor* ALabGenerator::InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale)
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, bpName));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return nullptr;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return nullptr;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<AActor>(GeneratedBP->GeneratedClass,
			FTransform{
				rotation,
				location,
				scale }, SpawnParams);
}

void ALabGenerator::GenerateDoorMeshes()
{
	std::for_each(begin(doors), end(doors),
		[&](LabBlock* labBlock)
		{
			const UStaticMeshSocket* doorSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("DoorE");
			if (doorSocket)
				InstanceBP(TEXT("/Game/Blueprints/DoorActor_BP.DoorActor_BP")
					, doorSocket->RelativeLocation  + tiles[labBlock->GetIndex()]->mesh->GetComponentLocation()
					, doorSocket->RelativeRotation + tiles[labBlock->GetIndex()]->mesh->GetComponentRotation()
					, doorSocket->RelativeScale );
		});
}

void ALabGenerator::GenerateKeyMeshes()
{
	std::for_each(begin(keys), end(keys),
		[&](LabBlock* labBlock)
		{
			const UStaticMeshSocket* nailSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Nail0");
			const UStaticMeshSocket* keySocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Key0");
			if (keySocket) {
				InstanceBP(TEXT("/Game/Blueprints/KeyPickUpActor_BP.KeyPickUpActor_BP")
					, { 0,0,0 }, FRotator::ZeroRotator, {2.f, 2.f, 2.f})->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("Key0"));
			}
			if(nailSocket){
				InstanceBP(TEXT("/Game/Blueprints/Nail_BP.Nail_BP")
					, {0,0,0})->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Nail0"));//
			}
		});
}

void ALabGenerator::GenerateHintMeshes()
{
	std::for_each(begin(hintClockPos), end(hintClockPos),
		[&](LabBlock* labBlock)
		{
			const UStaticMeshSocket* hintSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Hint0");
			if (hintSocket) {
				AActor* actor = InstanceBP(TEXT("/Game/Blueprints/HintClock_BP.HintClock_BP")
					, { 0,0,0 }, FRotator{ 0,0,0 });
				actor->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Hint0"));
				AHintDecalActor* hint = Cast<AHintDecalActor>(actor);
				hint->kind = static_cast<TypeHint>(labBlock->GetHintClockDir());
				hint->clockOrder = static_cast<ClockOrder>(labBlock->GetHintClockNb());
				hint->OnRep_UpdateMaterial();
				hint->OnRep_UpdateMaterialOrder();
			}
		});
}

void ALabGenerator::InitKeys()
{
	std::for_each(begin(doors), end(doors),
		[&](LabBlock* labBlock)
		{
			float spawnLuck = -0.5;
			bool variant = true;
			std::vector<LabBlock*> queue;
			std::vector<LabBlock*> alreadyChecked;

			LabBlock* currentNode = labBlock->GetNeighborNorth();
			while (variant) {
				if (seed.GetFraction() < spawnLuck)
					variant = false;
				spawnLuck += 0.03;
				alreadyChecked.push_back(currentNode);

				if (currentNode->GetNeighborNorth() != nullptr
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborNorth()) == end(alreadyChecked)
				&& !currentNode->GetNeighborNorth()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborNorth());

				if (currentNode->GetNeighborSouth() != nullptr
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborSouth()) == end(alreadyChecked)
				&& !currentNode->GetNeighborSouth()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborSouth());

				if (currentNode->GetNeighborEast() != nullptr
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborEast()) == end(alreadyChecked)
				&& !currentNode->GetNeighborEast()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborEast());

				if (currentNode->GetNeighborWest() != nullptr
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborWest()) == end(alreadyChecked)
				&& !currentNode->GetNeighborWest()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborWest());

				if (queue.size() == 0) {
					variant = false;
					continue;
				}
				currentNode = queue.back();

			}
			currentNode->SetHasKey(true);
			keys.push_back(currentNode);
			
		});
}

void ALabGenerator::InitHints()
{
	int sectionCounter = 0;
	for (int i = 0; i < puzzleRooms.Num(); ++i) {
		if (puzzleRooms[i]->IsA(AClockPuzzleRoom::StaticClass())) {
			AClockPuzzleRoom* clockRoom = Cast<AClockPuzzleRoom>(puzzleRooms[i]);

			std::vector<LabBlock*> alreadyChecked;
			LabBlock* currentNode = tilesBeginSection[sectionCounter++];
			std::vector<LabBlock*> queue;
			for (int clockId = 0; clockId < clockRoom->nbClocks; ++clockId)
			{
				float spawnLuck = -0.5;
				bool variant = true;

				while (variant) {
					if (seed.GetFraction() < spawnLuck)
						variant = false;
					spawnLuck += 0.03;
					alreadyChecked.push_back(currentNode);

					if (currentNode->GetNeighborNorth() != nullptr
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborNorth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborNorth());

					if (currentNode->GetNeighborSouth() != nullptr
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborSouth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborSouth());

					if (currentNode->GetNeighborEast() != nullptr
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborEast()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborEast());

					if (currentNode->GetNeighborWest() != nullptr
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborWest()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborWest());

					if (queue.size() == 0) {
						variant = false;
						continue;
					}
					currentNode = queue.back();
					queue.pop_back();
				}
				hintClockPos.push_back(currentNode);
				currentNode->SetHasHint(true);
				currentNode->SetHintClockDir(int(clockRoom->solutions[clockId]));
				currentNode->SetHintClockNb(int(clockId));
			}
		}
	}

}

void ALabGenerator::CreateStartRoom()
{
	int randomCol = seed.GetUnsignedInt() % width;
	labBlocks[GetIndex( randomCol, 0)].SetWallNorth(false);
	tilesBeginSection.push_back(&labBlocks[GetIndex(randomCol, 0)]);
	spawnRoom = GetWorld()->SpawnActor<ASpawnRoom>(ASpawnRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -(randomCol) * LabBlock::assetSize,LabBlock::assetSize ,0 }, FVector{1.f,1.f,1.f}));
	spawnRoom->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
}

void ALabGenerator::CreatePuzzlesRoom()
{
	int randomColEnd = seed.GetUnsignedInt() % width;

	std::for_each(bandes.begin(), bandes.end(),
		[&](int bande) {
			int randomCol = seed.GetUnsignedInt() % width;
			labBlocks[GetIndex(randomCol,bande - 1)].SetWallSouth(false);
			labBlocks[GetIndex(randomCol,bande + 2)].SetWallNorth(false);

			tilesBeginSection.push_back(&labBlocks[GetIndex(randomCol, bande + 2)]);
			//SOLUTION TEMPORAIRE
			AClockPuzzleRoom* puzzleRoom = GetWorld()->SpawnActor<AClockPuzzleRoom>(AClockPuzzleRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -randomCol * LabBlock::assetSize, -LabBlock::assetSize* bande, 0 }, FVector{ 1.f, 1.f, 1.f }));
			puzzleRoom->InitPuzzle(seed);
			puzzleRooms.Add(puzzleRoom);
			puzzleRoom->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			//
		});

	labBlocks[GetIndex(randomColEnd, height - 1)].SetWallSouth(false);
	
	APuzzleRoom* puzzleRoomEnd = GetWorld()->SpawnActor<APuzzleRoom>(APuzzleRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -randomColEnd * LabBlock::assetSize  , -LabBlock::assetSize * height, 0}, FVector{ 1.f, 1.f, 1.f }));

	puzzleRooms.Add(puzzleRoomEnd);
	puzzleRoomEnd->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

}




