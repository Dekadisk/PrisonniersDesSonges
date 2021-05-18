#include "LabGenerator.h"
#include "Tile.h" 
#include "Room.h"

#include "DrawDebugHelpers.h"
#include <algorithm>
#include "LabyrinthGameModeBase.h"
#include "Engine/StaticMeshSocket.h"
#include "NavigationSystem.h"
#include "AIEnemyTargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Math/Rotator.h"
#include "LabyrinthGameInstance.h"
#include "MonsterCharacter.h"
#include "NavMesh/NavMeshBoundsVolume.h"

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

	static ConstructorHelpers::FClassFinder<ASpawnRoom> SpawnRoom_BP_F(TEXT("/Game/Blueprints/SpawnRoom_BP"));
	static ConstructorHelpers::FClassFinder<ATile> Tile_BP_F(TEXT("/Game/Blueprints/Tile_BP"));
	static ConstructorHelpers::FClassFinder<ADebugMesh> BP_DebugPlane_F(TEXT("/Game/Blueprints/DEBUG/BP_DebugPlane"));
	static ConstructorHelpers::FClassFinder<ADoorActor> DoorActor_BP_F(TEXT("/Game/Blueprints/DoorActor_BP"));
	static ConstructorHelpers::FClassFinder<AKeyPickUpActor> KeyPickUpActor_BP_F(TEXT("/Game/Blueprints/KeyPickUpActor_BP"));
	static ConstructorHelpers::FClassFinder<AActor> Nail_BP_F(TEXT("/Game/Blueprints/Nail_BP"));
	static ConstructorHelpers::FClassFinder<ACachette> Cachette_BP_F(TEXT("/Game/Blueprints/Cachette_BP"));
	static ConstructorHelpers::FClassFinder<AHintDecalActor> HintClock_BP_F(TEXT("/Game/Blueprints/HintClock_BP"));
	static ConstructorHelpers::FClassFinder<ATrumpetDecorator> Trumpet_BP_F(TEXT("/Game/Blueprints/Trumpet_BP"));
	static ConstructorHelpers::FClassFinder<ALookAtTrigger> LookAtTrigger_BP_F(TEXT("/Game/Blueprints/LookAtTrigger_BP"));
	static ConstructorHelpers::FClassFinder<ARabbitDecorator> Rabbit_BP_F(TEXT("/Game/Blueprints/Rabbit_BP"));
	static ConstructorHelpers::FClassFinder<ABicycleDecorator> Bicycle_BP_F(TEXT("/Game/Blueprints/Bicycle_BP"));
	static ConstructorHelpers::FClassFinder<AFrameDecorator> Frame_BP_F(TEXT("/Game/Blueprints/Frame_BP"));
	static ConstructorHelpers::FClassFinder<AMushroomDecorator> Mushroom_BP_F(TEXT("/Game/Blueprints/Mushroom_BP"));
	static ConstructorHelpers::FClassFinder<ARockDecorator> Rock_BP_F(TEXT("/Game/Blueprints/Rock_BP"));
	static ConstructorHelpers::FClassFinder<AActor> FireTorch_BP_F(TEXT("/Game/CustomMaterials/FireTorch_BP"));
	static ConstructorHelpers::FClassFinder<AClockPuzzleRoom> ClockPuzzleRoom_BP_F(TEXT("/Game/Blueprints/ClockPuzzleRoom_BP"));
	static ConstructorHelpers::FClassFinder<ABellPuzzleRoom> BellPuzzleRoom_BP_F(TEXT("/Game/Blueprints/BellPuzzleRoom_BP"));
	static ConstructorHelpers::FClassFinder<ALampPuzzleRoom> LampPuzzleRoom_BP_F(TEXT("/Game/Blueprints/LampPuzzleRoom_BP"));
	static ConstructorHelpers::FClassFinder<AActor> EndPuzzleRoom_BP_F(TEXT("/Game/Blueprints/EndPuzzleRoom_BP"));
	SpawnRoom_BP = SpawnRoom_BP_F.Class;
	Tile_BP = Tile_BP_F.Class;
	BP_DebugPlane = BP_DebugPlane_F.Class;
	DoorActor_BP = DoorActor_BP_F.Class;
	KeyPickUpActor_BP = KeyPickUpActor_BP_F.Class;
	Nail_BP = Nail_BP_F.Class;
	Cachette_BP = Cachette_BP_F.Class;
	HintClock_BP = HintClock_BP_F.Class;
	Trumpet_BP = Trumpet_BP_F.Class;
	LookAtTrigger_BP = LookAtTrigger_BP_F.Class;
	Rabbit_BP = Rabbit_BP_F.Class;
	Bicycle_BP = Bicycle_BP_F.Class;
	Frame_BP = Frame_BP_F.Class;
	Mushroom_BP = Mushroom_BP_F.Class;
	Rock_BP = Rock_BP_F.Class;
	FireTorch_BP = FireTorch_BP_F.Class;
	ClockPuzzleRoom_BP = ClockPuzzleRoom_BP_F.Class;
	BellPuzzleRoom_BP = BellPuzzleRoom_BP_F.Class;
	LampPuzzleRoom_BP = LampPuzzleRoom_BP_F.Class;
	EndPuzzleRoom_BP = EndPuzzleRoom_BP_F.Class;

}
void ALabGenerator::Tick(float some_float) {
	UpdateInfluenceMap();
	PropagateInfluenceMap();
	//FlushDebugStrings(GetWorld());
	//DrawDebugInfluenceMap();
}
// Called when the game starts or when spawned
void ALabGenerator::BeginPlay()
{
	Super::BeginPlay();

	int iseed = Cast<ULabyrinthGameInstance>(GetGameInstance())->currentPartyDataForSave->seedUsed;
	if (iseed == 0)
	{

	}
	else
	{
		seed.Initialize(FName(FString::FromInt(iseed)));
	}
	
	if (!HasAuthority())
		int a = 0;

	InitSize();
	InitBlocks();
	CreateMaze();
	RemoveImpasse();
	RemoveLines();

	CreateStartRoom();   //CONTAIN HAS AUTORITY
	CreatePuzzlesRoom(); //CONTAIN HAS AUTORITY
	InitObjects();
	InitPuzzleObjects();
	Conversion2Types();
	GenerateMazeMesh();
	GenerateDecorationMeshes();

	if (HasAuthority()) {
		GenerateDoorMeshes();
		GenerateObjectsMeshes();
		GenerateHintMeshes();
		GeneratePuzzleObjectsMeshes();
		GenerateTargetPoint();
		SpawnNavMesh();
		UpdateInfluenceMap();
		PropagateInfluenceMap();
		ALabyrinthGameModeBase* gamemode = Cast<ALabyrinthGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		gamemode->labGeneratorDone = true;
	}
	SpawnMonster();
	
	if(HasAuthority())
		Cast<ULabyrinthGameInstance>(GetGameInstance())->SetStartTime();
	//gamemode->SpawnPlayers();
	//DEBUG
	//DrawDebugLabGraph();
	//DrawDebugLabGraph();
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
void ALabGenerator::SpawnMonster() {
	LabBlock* summon_tile;
	if (doors.size() > 0 && doors[0]->GetSectionId() == 0) {
		summon_tile = doors[0];
		
	}
	else{
		summon_tile = tilesBeginSection[1];
	}

	AMonsterCharacter * monster = Cast<AMonsterCharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), AMonsterCharacter::StaticClass()));
	monster->SetActorLocation(summon_tile->GetGlobalPos());
};

void ALabGenerator::SpawnNavMesh() {

	int Sum{};
	for (int i = 0; i < nbSubSections.Num(); i++)
		Sum += nbSubSections[i];

	FVector positionCentre = FVector{ -225.f * width, -0.5f*(nbSubSections.Num()*1100+Sum* LabBlock::assetSize *subSectionSize), 0.f};

	AActor* NavMesh = UGameplayStatics::GetActorOfClass(GetWorld(), ANavMeshBoundsVolume::StaticClass());
	NavMesh->GetRootComponent()->SetMobility(EComponentMobility::Movable);
	NavMesh->SetActorLocation(positionCentre, false);
	NavMesh->SetActorRelativeScale3D(FVector{ width+1.f, nbSubSections.Num() * 2.f + Sum * subSectionSize+5.f, 1.0f });
	NavMesh->GetRootComponent()->UpdateBounds();
	UNavigationSystemV1* NavigationArea = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	NavigationArea->UNavigationSystemV1::OnNavigationBoundsUpdated(Cast<ANavMeshBoundsVolume>(NavMesh));

}

void ALabGenerator::UpdateInfluenceMap()
{
	for (ATile* tile : tiles) {
		tile->UpdateInfluenceSources();
	}
}

void ALabGenerator::PropagateInfluenceMap()
{
	int radius = 3;

	for (ATile* tile : tiles) {
		tile->inf_final = 1.0;
	}
	for (LabBlock& labBlock : labBlocks) {
		std::vector<LabBlock*> alreadyInfluenced;
		std::vector<LabBlock*> toCheckNeighbors;
		toCheckNeighbors.push_back(&labBlock);

		float sumCurrent = 0.f;
		for (TPair<InfluenceGroup, float>& pair : tiles[labBlock.GetIndex()]->inf_values)
			sumCurrent += pair.Value;
		tiles[labBlock.GetIndex()]->inf_final += sumCurrent;

		for (int i = 0; i < radius; ++i) {
			std::vector<LabBlock*> neighbors;
			for (int bToCheck = 0; bToCheck < toCheckNeighbors.size(); ++bToCheck) {
				if (toCheckNeighbors.back()->GetNeighborNorth()!=nullptr 
					&& std::find(alreadyInfluenced.begin(),alreadyInfluenced.end(), toCheckNeighbors.back()->GetNeighborNorth()) == alreadyInfluenced.end())
					neighbors.push_back(toCheckNeighbors.back()->GetNeighborNorth());
				if (toCheckNeighbors.back()->GetNeighborEast() != nullptr
					&& std::find(alreadyInfluenced.begin(), alreadyInfluenced.end(), toCheckNeighbors.back()->GetNeighborEast()) == alreadyInfluenced.end())
					neighbors.push_back(toCheckNeighbors.back()->GetNeighborEast());
				if (toCheckNeighbors.back()->GetNeighborSouth() != nullptr
					&& std::find(alreadyInfluenced.begin(), alreadyInfluenced.end(), toCheckNeighbors.back()->GetNeighborSouth()) == alreadyInfluenced.end())
					neighbors.push_back(toCheckNeighbors.back()->GetNeighborSouth());
				if (toCheckNeighbors.back()->GetNeighborWest() != nullptr
					&& std::find(alreadyInfluenced.begin(), alreadyInfluenced.end(), toCheckNeighbors.back()->GetNeighborWest()) == alreadyInfluenced.end())
					neighbors.push_back(toCheckNeighbors.back()->GetNeighborWest());
				alreadyInfluenced.push_back(toCheckNeighbors.back());
				toCheckNeighbors.pop_back();
			}
			for (LabBlock* neighbor : neighbors) {
				tiles[neighbor->GetIndex()]->inf_final += sumCurrent*(radius-i-1)/radius;
				toCheckNeighbors.push_back(neighbor);
			}
		}
	}
}

void ALabGenerator::InitBlocks() {
	
	for (int i = 0; i < width; i++)
	{
		for (int j = 0; j < height; j++) {
			labBlocks.push_back(LabBlock(i,j,GetIndex(i,j)));
			if ((std::find(begin(bandes), end(bandes), j) != end(bandes)
				|| std::find(begin(bandes), end(bandes), j - 1) != end(bandes)) && i != 0)
				labBlocks.back().setLocked(true);
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
	if (availableNeighbors.size() > 0)
	    return availableNeighbors.at(seed.GetUnsignedInt() % availableNeighbors.size());
	
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
	    if (i != -1 && !labBlocks[i].getVisited() && !labBlocks[i].IsLocked())
	        neighbors.push_back(&labBlocks[i]);
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
	for (auto& block : labBlocks)
		typeLabBlocks.push_back(Converter(block));
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
			ATile* tileIteration = Cast<ATile>(InstanceBP(Tile_BP/*TEXT("/Game/Blueprints/Tile_BP.Tile_BP")*/, FVector{ -i * LabBlock::assetSize ,-j * LabBlock::assetSize,0 }, FRotator::ZeroRotator, { 1.f,1.f,1.f }));
			// GetWorld()->SpawnActor<ATile>(ATile::StaticClass(), FTransform(FQuat::Identity, FVector{ -i * LabBlock::assetSize ,-j * LabBlock::assetSize,0 }, FVector{ 1.f,1.f,1.f }));
			tileIteration->kind = typeLabBlocks[i * height + j];
			tileIteration->UpdateMesh();
			tileIteration->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			tileIteration->index = GetIndex(i, j);
			tiles.Add(tileIteration);
			/*if(typeLabBlocks[i * height + j] > 0 && typeLabBlocks[i * height + j]<16) tiles.Add(tileIteration);
			else { tiles.Add(nullptr); }*/
		}
	}
}
void ALabGenerator::DrawDebugInfluenceMap() {
	float max_inf = 0.f;
	for (ATile* tile : tiles) {
		max_inf = FMath::Max(max_inf, tile->inf_final);
	}
	if (debugMeshInfMap.Num() == 0)
	{
		for (ATile* tile : tiles) {
			debugMeshInfMap.Add(Cast<ADebugMesh>(InstanceBP(BP_DebugPlane/*TEXT("/Game/Blueprints/DEBUG/BP_DebugPlane.BP_DebugPlane")*/, tile->GetActorLocation(), tile->GetActorRotation(), { LabBlock::assetSize /2,LabBlock::assetSize/2 ,LabBlock::assetSize/2 })));
			UMaterialInstanceDynamic* dynamicMaterial = UMaterialInstanceDynamic::Create(debugMeshInfMap.Last()->mesh->GetMaterial(0), debugMeshInfMap.Last()->mesh);
			debugMeshInfMap.Last()->mesh->SetMaterial(0, dynamicMaterial);			
		}
	}
	int varB = 0;
	for (ATile* tile : tiles) {
		if (!tile)
			continue;
		DrawDebugString(GetWorld(), tile->GetActorLocation(), FString::SanitizeFloat(tile->inf_final));
		float s = tile->inf_final / max_inf;
		FLinearColor color{ s,0.f,(1.f-s),1.f };//
		Cast<UMaterialInstanceDynamic>(debugMeshInfMap[varB]->mesh->GetMaterial(0))->SetVectorParameterValue("BaseColor", color);
		++varB;
	}
}
void ALabGenerator::DrawDebugLabGraph()
{
	for (LabBlock& labBlock : labBlocks) {
		if (labBlock.GetHasKey()) {
			DrawDebugSphere(GetWorld(), labBlock.GetGlobalPos(), 20, 4, FColor::Yellow, true);
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetGlobalPos() + FVector{ 0,0,300 }, FColor::Yellow, true);
		}
		if (labBlock.GetHasHint()) {
			DrawDebugSphere(GetWorld(), labBlock.GetGlobalPos(), 30, 4, FColor::Purple, true);
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetGlobalPos() + FVector{ 0,0,300 }, FColor::Purple, true);
		}
		if (labBlock.GetHasBell()) {
			DrawDebugSphere(GetWorld(), labBlock.GetGlobalPos(), 40, 4, FColor::Cyan, true);
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetGlobalPos() + FVector{ 0,0,300 }, FColor::Cyan, true);
		}
		if (labBlock.GetHasLamp()) {
			DrawDebugSphere(GetWorld(), labBlock.GetGlobalPos(), 50, 4, FColor::Green, true);
			DrawDebugLine(GetWorld(), labBlock.GetGlobalPos(), labBlock.GetGlobalPos() + FVector{ 0,0,300 }, FColor::Green, true);
		}
		if (labBlock.GetHasDoor())
			DrawDebugBox(GetWorld(), labBlock.GetGlobalPos(), { LabBlock::assetSize / 2,LabBlock::assetSize / 6,LabBlock::assetSize / 2 }, FColor(0, 255, 0), true);
		if(!labBlock.IsLocked())
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

AActor* ALabGenerator::InstanceBP(TSubclassOf<UObject> blueprint, FVector location, FRotator rotation, FVector scale)
{
	/*const TCHAR* ziTChar = *bpName;
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, ziTChar));

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
	*/
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<AActor>(blueprint,
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
			{
				AUsableActor* door = Cast<AUsableActor>(InstanceBP(DoorActor_BP/*TEXT("/Game/Blueprints/DoorActor_BP.DoorActor_BP")*/
					, doorSocket->RelativeLocation + tiles[labBlock->GetIndex()]->mesh->GetComponentLocation()
					, doorSocket->RelativeRotation + tiles[labBlock->GetIndex()]->mesh->GetComponentRotation()
					, doorSocket->RelativeScale));
				
				FPE_PuzzleEvent pedoor;
				FPE_Subtitle subtitle;
				subtitle.Subtitle = FText().FromString("J'ai besoin d'une clef pour ouvrir la porte");
				subtitle.Duration = 4;
				FPE_SubtitleSeq seq;
				seq.Broadcast = false;
				seq.Delay = 0;
				seq.Sequence.Add(subtitle);
				pedoor.Subtitles.Subtitles.Add(seq);

				FPE_PuzzleEventMaster emdoor;
				emdoor.Event = pedoor;
				emdoor.Trigger = EPuzzleEventCheck::ForceOpen;
				emdoor.OnlyOnce = false;
				door->PuzzleEvents.Add(emdoor);
			}
				

		});
}

void ALabGenerator::GenerateObjectsMeshes()
{
	std::for_each(begin(keys), end(keys),
		[&](LabBlock* labBlock)
		{
			const UStaticMeshSocket* nailSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Nail0");
			const UStaticMeshSocket* keySocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Key0");
			FTransform nailTransform;
			FTransform keyTransform;

			nailSocket->GetSocketTransform(nailTransform, tiles[labBlock->GetIndex()]->mesh);
			keySocket->GetSocketTransform(keyTransform, tiles[labBlock->GetIndex()]->mesh);
			if (keySocket) {
				InstanceBP(KeyPickUpActor_BP/*TEXT("/Game/Blueprints/KeyPickUpActor_BP.KeyPickUpActor_BP")*/
					, keyTransform.GetLocation(), keyTransform.GetRotation().Rotator(), keyTransform.GetScale3D());//->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, true), TEXT("Key0"));
			}
			if(nailSocket){
				InstanceBP(Nail_BP/*TEXT("/Game/Blueprints/Nail_BP.Nail_BP")*/
					, nailTransform.GetLocation(), nailTransform.GetRotation().Rotator(), nailTransform.GetScale3D());//->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Nail0"));//
			}
		});
	std::for_each(begin(hidingSpots), end(hidingSpots),
		[&](LabBlock* labBlock)
		{
			const UStaticMeshSocket* hidingSpotSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("HidingSpot0");
			FTransform hidingSpotTransform;

			hidingSpotSocket->GetSocketTransform(hidingSpotTransform, tiles[labBlock->GetIndex()]->mesh);
			if (hidingSpotSocket) {
				InstanceBP(Cachette_BP/*TEXT("/Game/Blueprints/Cachette_BP.Cachette_BP")*/
					, hidingSpotTransform.GetLocation(), hidingSpotTransform.GetRotation().Rotator(), hidingSpotTransform.GetScale3D());
			}
		});
}

void ALabGenerator::GenerateHintMeshes()
{
	for (int i = 0; i < hintClockPos.size(); ++i) {
		std::for_each(begin(hintClockPos[i]), end(hintClockPos[i]),
			[&](LabBlock* labBlock)
			{
				const UStaticMeshSocket* hintSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Hint0");
				if (hintSocket) {
					FTransform transform;

					hintSocket->GetSocketTransform(transform, tiles[labBlock->GetIndex()]->mesh);
					AActor* actor = InstanceBP(HintClock_BP/*TEXT("/Game/Blueprints/HintClock_BP.HintClock_BP")*/
						, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
					//actor->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Hint0"));
					AHintDecalActor* hint = Cast<AHintDecalActor>(actor);
					hint->kind = static_cast<TypeHint>(labBlock->GetHintClockDir());
					hint->clockOrder = static_cast<ClockOrder>(labBlock->GetHintClockNb());
					hint->OnRep_UpdateMaterial();
					hint->OnRep_UpdateMaterialOrder();
				}
			});
	}
}

void ALabGenerator::GenerateTargetPoint()
{
	FVector Location(0.0f, 0.0f, 0.0f);
	FRotator Rotation(0.0f, 0.0f, 0.0f);
	FActorSpawnParameters SpawnInfo;
	std::for_each(begin(labBlocks), end(labBlocks),
		[&](LabBlock& labBlock)
		{
			if (!labBlock.IsLocked() || labBlock.HasNeighbors()) {
				AAIEnemyTargetPoint* targetPoint = GetWorld()->SpawnActor<AAIEnemyTargetPoint>(Location, Rotation, SpawnInfo);
				targetPoint->AttachToComponent(tiles[labBlock.GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("TargetPoint0"));
				targetPoint->Tags.Add(FName(FString::FromInt(labBlock.GetSectionId())));
			}
		});

	/*int puzzleRoomCounter = 0;
	for (APuzzleRoom* puzzleRoom : puzzleRooms)
	{
		AAIEnemyTargetPoint* targetPoint = GetWorld()->SpawnActor<AAIEnemyTargetPoint>(Location, Rotation, SpawnInfo);
		targetPoint->AttachToComponent(puzzleRoom->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("TargetPoint0"));
		targetPoint->Tags.Add(FName(FString::FromInt(puzzleRoomCounter++)));
	}
	AAIEnemyTargetPoint* targetPoint = GetWorld()->SpawnActor<AAIEnemyTargetPoint>(Location, Rotation, SpawnInfo);
	targetPoint->AttachToComponent(spawnRoom->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("TargetPoint0"));
	targetPoint->Tags.Add(FName(FString::FromInt(0)));*/
	
	// Comes from "Bell" branch.
	/*std::for_each(begin(hintBellPos), end(hintBellPos),
		[&](LabBlock* labBlock)
		{
			const UStaticMeshSocket* bellSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Bell0");
			if (bellSocket) {
				AActor* actor = InstanceBP(TEXT("/Game/Blueprints/BellPuzzleActor_BP.BellPuzzleActor_BP")
					, { 0,0,0 }, FRotator{ 0,0,0 });
				actor->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Bell0"));
			}
		});*/
}

void ALabGenerator::InitObjects()
{
	std::for_each(begin(doors), end(doors),
		[&](LabBlock* labBlock)
		{
			// SET KEYS
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

				if (currentNode->GetNeighborNorth() != nullptr && labBlocks[currentNode->GetNeighborNorth()->GetIndex()].IsLocked() == false
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborNorth()) == end(alreadyChecked)
				&& !currentNode->GetNeighborNorth()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborNorth());

				if (currentNode->GetNeighborSouth() != nullptr && labBlocks[currentNode->GetNeighborSouth()->GetIndex()].IsLocked() == false
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborSouth()) == end(alreadyChecked)
				&& !currentNode->GetNeighborSouth()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborSouth());

				if (currentNode->GetNeighborEast() != nullptr && labBlocks[currentNode->GetNeighborEast()->GetIndex()].IsLocked() == false
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborEast()) == end(alreadyChecked)
				&& !currentNode->GetNeighborEast()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborEast());

				if (currentNode->GetNeighborWest() != nullptr && labBlocks[currentNode->GetNeighborWest()->GetIndex()].IsLocked() == false
				&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborWest()) == end(alreadyChecked)
				&& !currentNode->GetNeighborWest()->GetHasDoor())
					queue.push_back(currentNode->GetNeighborWest());

				if (queue.size() == 0) {
					variant = false;
					continue;
				}
				currentNode = queue.back();
				queue.pop_back();

			}
			currentNode->SetHasKey(true);
			keys.push_back(currentNode);
			//
			//SET HIDINGSPOT
			currentNode = nullptr;
			std::vector<LabBlock*> corners;
			for (LabBlock* labBlock : alreadyChecked) {
				if (labBlock->GetNbWalls() == 3)
					corners.push_back(labBlock);
			}
			if (corners.size() > 0) {
				currentNode = corners[seed.GetUnsignedInt() % corners.size()];
				currentNode->SetHasHidingSpot(true);
				hidingSpots.push_back(currentNode);
			}
			//
			
		});
}

void ALabGenerator::GeneratePuzzleObjectsMeshes() {
	for (int i = 0; i < puzzleRoomsType.size(); ++i) {
		if (puzzleRoomsType[i] == PuzzleType::Bell) {
			Cast<ABellPuzzleRoom>(puzzleRooms[i])->CreateBells(bellPos[i],bellHintPos[i][0], tiles);
		}
		if (puzzleRoomsType[i] == PuzzleType::Clock) {
			Cast<AClockPuzzleRoom>(puzzleRooms[i])->CreateClocks(clockPos[i], tiles);
		}
		if (puzzleRoomsType[i] == PuzzleType::Lamp) {
			Cast<ALampPuzzleRoom>(puzzleRooms[i])->CreateLamps(lampPos[i], tiles);
		}
	}
}

void ALabGenerator::GenerateDecorationMeshes()
{
	// Kids Decoration

	int idsKidDecoration[4] = {0,0,0,0};
	for (int i = 0; i < 4; ++i) {
		do {
			idsKidDecoration[i] = seed.GetUnsignedInt() % labBlocks.size();
		} while (labBlocks[idsKidDecoration[i]].IsLocked());
		ATile* tile = tiles[idsKidDecoration[i]];
		TArray<FName> socketNames = tile->mesh->GetAllSocketNames();
		for (FName socketName : socketNames) {
			if (socketName.ToString().Contains("Trumpet") && i==0) {
				const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
				FTransform transform;
				socket->GetSocketTransform(transform, tile->mesh);

				ATrumpetDecorator* trumpet = Cast<ATrumpetDecorator>(InstanceBP(Trumpet_BP/*TEXT("/Game/Blueprints/Trumpet_BP.Trumpet_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
				trumpet->setKind(seed.GetUnsignedInt());
				ALookAtTrigger* lookat = Cast<ALookAtTrigger>(InstanceBP(LookAtTrigger_BP/*TEXT("/Game/Blueprints/LookAtTrigger_BP.LookAtTrigger_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()*2.f));

				FPE_PuzzleEvent pedec;
				FPE_Subtitle subtitle;
				subtitle.Subtitle = FText().FromString("I recognize this trumpet...");
				subtitle.Duration = 4;
				FPE_SubtitleSeq seq;
				seq.Broadcast = false;
				seq.Delay = 0;
				seq.Sequence.Add(subtitle);
				pedec.Subtitles.Subtitles.Add(seq);

				FPE_PuzzleEventMaster emdec;
				emdec.Event = pedec;
				emdec.Trigger = EPuzzleEventCheck::LookAt;
				emdec.OnlyOnce = false;
				lookat->PuzzleEvents.Add(emdec);
				lookat->maxDist = 100;
			}
			if (socketName.ToString().Contains("Rabbit") && i == 1) {
				const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
				FTransform transform;
				socket->GetSocketTransform(transform, tile->mesh);

				ARabbitDecorator* rabbit = Cast<ARabbitDecorator>(InstanceBP(Rabbit_BP/*TEXT("/Game/Blueprints/Rabbit_BP.Rabbit_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
				rabbit->setKind(seed.GetUnsignedInt());

				ALookAtTrigger* lookat = Cast<ALookAtTrigger>(InstanceBP(LookAtTrigger_BP/*TEXT("/Game/Blueprints/LookAtTrigger_BP.LookAtTrigger_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D() * 2.f));

				FPE_PuzzleEvent pedec;
				FPE_Subtitle subtitle;
				subtitle.Subtitle = FText().FromString("This plush should be in our room...");
				subtitle.Duration = 4;
				FPE_SubtitleSeq seq;
				seq.Broadcast = false;
				seq.Delay = 0;
				seq.Sequence.Add(subtitle);
				pedec.Subtitles.Subtitles.Add(seq);

				FPE_PuzzleEventMaster emdec;
				emdec.Event = pedec;
				emdec.Trigger = EPuzzleEventCheck::LookAt;
				emdec.OnlyOnce = false;
				lookat->PuzzleEvents.Add(emdec);
				lookat->maxDist = 100;
			}
			if (socketName.ToString().Contains("Bicycle") && i == 2) {
				const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
				FTransform transform;
				socket->GetSocketTransform(transform, tile->mesh);

				ABicycleDecorator* bicycle = Cast<ABicycleDecorator>(InstanceBP(Bicycle_BP/*TEXT("/Game/Blueprints/Bicycle_BP.Bicycle_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
				bicycle->setKind(seed.GetUnsignedInt());
				ALookAtTrigger* lookat = Cast<ALookAtTrigger>(InstanceBP(LookAtTrigger_BP/*TEXT("/Game/Blueprints/LookAtTrigger_BP.LookAtTrigger_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D() * 2.f));

				FPE_PuzzleEvent pedec;
				FPE_Subtitle subtitle;
				subtitle.Subtitle = FText().FromString("That's a nice bicycle!");
				subtitle.Duration = 4;
				FPE_SubtitleSeq seq;
				seq.Broadcast = false;
				seq.Delay = 0;
				seq.Sequence.Add(subtitle);
				pedec.Subtitles.Subtitles.Add(seq);

				FPE_PuzzleEventMaster emdec;
				emdec.Event = pedec;
				emdec.Trigger = EPuzzleEventCheck::LookAt;
				emdec.OnlyOnce = false;
				lookat->PuzzleEvents.Add(emdec);
				lookat->maxDist = 100;
			}
			if (socketName.ToString().Contains("Frame") && i == 3) {
				const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
				FTransform transform;
				socket->GetSocketTransform(transform, tile->mesh);

				AFrameDecorator* frame = Cast<AFrameDecorator>(InstanceBP(Frame_BP/*TEXT("/Game/Blueprints/Frame_BP.Frame_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
				frame->setKind(seed.GetUnsignedInt());
				ALookAtTrigger* lookat = Cast<ALookAtTrigger>(InstanceBP(LookAtTrigger_BP/*TEXT("/Game/Blueprints/LookAtTrigger_BP.LookAtTrigger_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D() * 2.f));

				FPE_PuzzleEvent pedec;
				FPE_Subtitle subtitle;
				subtitle.Subtitle = FText().FromString("I hate these frames...");
				subtitle.Duration = 4;
				FPE_SubtitleSeq seq;
				seq.Broadcast = false;
				seq.Delay = 0;
				seq.Sequence.Add(subtitle);
				pedec.Subtitles.Subtitles.Add(seq);

				FPE_PuzzleEventMaster emdec;
				emdec.Event = pedec;
				emdec.Trigger = EPuzzleEventCheck::LookAt;
				emdec.OnlyOnce = false;
				lookat->PuzzleEvents.Add(emdec);
				lookat->maxDist = 100;
			}
		}
	}

	// Rocks and Mushroom decorations
		// And Torches

	float torchSpawnChances[3] = { 0.95f , 0.70f, 0.5f };
	for (ATile* tile : tiles) {
		if (tile == nullptr || tile->kind==0)
			continue;
		TArray<FName> socketNames = tile->mesh->GetAllSocketNames();
		bool hasTorch = false;
		float torchSpawnChance = 1.f;
		if(puzzleRooms.Num() > labBlocks[tile->index].GetSectionId())
			torchSpawnChance = torchSpawnChances[(int)puzzleRooms[labBlocks[tile->index].GetSectionId()]->difficulty];
		bool needTorch = seed.GetFraction() < torchSpawnChance;
		int torchSocket = seed.GetUnsignedInt() % 4;
		int torchSocketCounter = 0;
		for (FName socketName : socketNames) {
			if (socketName.ToString().Contains("Mushroom")) {
				const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
				FTransform transform;
				socket->GetSocketTransform(transform, tile->mesh);
				
				AMushroomDecorator* mushroom = Cast<AMushroomDecorator>(InstanceBP(Mushroom_BP/*TEXT("/Game/Blueprints/Mushroom_BP.Mushroom_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
				mushroom->setKind(seed.GetUnsignedInt());
				mushroom->Tags.Add(FName(FString::FromInt(labBlocks[tile->index].GetSectionId())));
			}
			if (socketName.ToString().Contains("LittleRock")) {
				const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
				FTransform transform;
				socket->GetSocketTransform(transform, tile->mesh);

				ARockDecorator* rock = Cast<ARockDecorator>(InstanceBP(Rock_BP/*TEXT("/Game/Blueprints/Rock_BP.Rock_BP")*/
					, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
				rock->setKind(seed.GetUnsignedInt());
				rock->Tags.Add(FName(FString::FromInt(labBlocks[tile->index].GetSectionId())));
			}
			if (socketName.ToString().Contains("Torch")){
				if (needTorch && hasTorch == false && torchSocketCounter==torchSocket) {
					hasTorch = true;
					const UStaticMeshSocket* socket = tile->mesh->GetSocketByName(socketName);
					FTransform transform;
					socket->GetSocketTransform(transform, tile->mesh);

					AActor* torch = InstanceBP(FireTorch_BP/*TEXT("/Game/CustomMaterials/FireTorch_BP.FireTorch_BP")*/
						, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
					torch->Tags.Add(FName(FString::FromInt(labBlocks[tile->index].GetSectionId())));
				}
				torchSocketCounter++;
			}
		}
	}

}

void ALabGenerator::InitPuzzleObjects()
{
	int sectionCounter = 0;
	for (int i = 0; i < puzzleRoomsType.size(); ++i) {
		//CLOCK HINT
		if (puzzleRoomsType[i] == PuzzleType::Clock) {
			AClockPuzzleRoom* clockRoom = nullptr;
			if(HasAuthority()) clockRoom = Cast<AClockPuzzleRoom>(puzzleRooms[i]);

			std::vector<LabBlock*> alreadyChecked;
			LabBlock* currentNode = tilesBeginSection[sectionCounter];
			std::vector<LabBlock*> queue;

			int32 nbClock = int32(round(float(i) / bandes.size() * 2.f)) + 2;
			for (int clockId = 0; clockId < nbClock; ++clockId)
			{
				float spawnLuck = -0.5;
				bool variant = true;

				while (variant) {
					if (seed.GetFraction() < spawnLuck && !currentNode->GetHasHint())
						variant = false;
					spawnLuck += 0.03;
					alreadyChecked.push_back(currentNode);

					if (currentNode->GetNeighborNorth() != nullptr && labBlocks[currentNode->GetNeighborNorth()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborNorth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborNorth());

					if (currentNode->GetNeighborSouth() != nullptr && labBlocks[currentNode->GetNeighborSouth()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborSouth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborSouth());

					if (currentNode->GetNeighborEast() != nullptr && labBlocks[currentNode->GetNeighborEast()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborEast()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborEast());

					if (currentNode->GetNeighborWest() != nullptr && labBlocks[currentNode->GetNeighborWest()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborWest()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborWest());

					if (queue.size() == 0) {
						variant = false;
						do {
							currentNode = *(alreadyChecked.begin() + seed.GetUnsignedInt() % alreadyChecked.size());
						} while (std::find(hintClockPos[i].begin(), hintClockPos[i].end(), currentNode) != end(hintClockPos[i]));
						continue;
					}
					currentNode = queue.back();
					queue.pop_back();
				}
				hintClockPos[i].push_back(currentNode);
				currentNode->SetHasHint(true);
				if(clockRoom != nullptr)currentNode->SetHintClockDir(int(clockRoom->solutions[clockId]));
				currentNode->SetHintClockNb(int(clockId));
			}
		}
		// LAMPS
		if (puzzleRoomsType[i] == PuzzleType::Lamp) {
			LabBlock* currentNode = tilesBeginSection[sectionCounter];
			int minX, minY, maxX, maxY;
			minX = 0;
			minY = currentNode->GetY();
			maxX = minX + width - 1;
			maxY = minY + nbSubSections[i] * subSectionSize - 1;

			std::vector<std::vector<LabBlock*>> lamps;
			ALampPuzzleRoom* lampRoom = nullptr;
			if (HasAuthority()) lampRoom = Cast<ALampPuzzleRoom>(puzzleRooms[i]);

			int32 nbGroup = int32(round(float(i) / bandes.size() * 2.f)) + 1;
			for (int groupId = 0; groupId < nbGroup; ++groupId) {
				lamps.emplace_back(std::vector<LabBlock*>{});
				do {
					int X = seed.GetUnsignedInt() % (maxX - minX) + minX;
					int Y = seed.GetUnsignedInt() % (maxY - minY) + minY;
					lamps[groupId].clear();
					lamps[groupId].push_back(&labBlocks[GetIndex(X, Y)]);
					lamps[groupId].push_back(&labBlocks[GetIndex(X, Y + 1)]);
					lamps[groupId].push_back(&labBlocks[GetIndex(X + 1, Y + 1)]);
					lamps[groupId].push_back(&labBlocks[GetIndex(X + 1, Y)]);
				} while (lamps[groupId][0]->IsLocked() || lamps[groupId][1]->IsLocked() || lamps[groupId][2]->IsLocked() || lamps[groupId][3]->IsLocked()
					|| lamps[groupId][0]->GetHasLamp() || lamps[groupId][1]->GetHasLamp() || lamps[groupId][2]->GetHasLamp() || lamps[groupId][3]->GetHasLamp());
				for (int j = 0; j < 4/*nbLampPerGroup*/; ++j) {
					lampPos[i].push_back(lamps[groupId][j]);
					switch (j) {
					case 0:
						lamps[groupId][j]->SetNeighborSouth(lamps[groupId][1]);
						lamps[groupId][j]->SetNeighborEast(lamps[groupId][3]);
						lamps[groupId][j]->SetWallSouth(false);
						lamps[groupId][j]->SetWallEast(false);
						break;
					case 1:
						lamps[groupId][j]->SetNeighborNorth(lamps[groupId][0]);
						lamps[groupId][j]->SetNeighborEast(lamps[groupId][2]);
						lamps[groupId][j]->SetWallNorth(false);
						lamps[groupId][j]->SetWallEast(false);
						break;
					case 3:
						lamps[groupId][j]->SetNeighborWest(lamps[groupId][0]);
						lamps[groupId][j]->SetNeighborSouth(lamps[groupId][2]);
						lamps[groupId][j]->SetWallSouth(false);
						lamps[groupId][j]->SetWallWest(false);
						break;
					case 2:
						lamps[groupId][j]->SetNeighborWest(lamps[groupId][1]);
						lamps[groupId][j]->SetNeighborNorth(lamps[groupId][3]);
						lamps[groupId][j]->SetWallNorth(false);
						lamps[groupId][j]->SetWallWest(false);
						break;
					}
					lamps[groupId][j]->SetHasLamp(true);
				}
			}
		}
		// CLOCKS
		if (puzzleRoomsType[i] == PuzzleType::Clock) {

			AClockPuzzleRoom* clockRoom = nullptr;
			if (HasAuthority()) clockRoom = Cast<AClockPuzzleRoom>(puzzleRooms[i]);

			std::vector<LabBlock*> alreadyChecked;
			LabBlock* currentNode = tilesBeginSection[sectionCounter];
			std::vector<LabBlock*> queue;

			int32 nbClock = int32(round(float(i) / bandes.size() * 2.f)) + 2;
			for (int clockId = 0; clockId < nbClock; ++clockId) //TODO replace 4
			{
				float spawnLuck = -0.5;
				bool variant = true;

				while (variant) {
					if (seed.GetFraction() < spawnLuck && !currentNode->GetHasClock())
						variant = false;
					spawnLuck += 0.03;
					alreadyChecked.push_back(currentNode);

					if (currentNode->GetNeighborNorth() != nullptr && labBlocks[currentNode->GetNeighborNorth()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborNorth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborNorth());

					if (currentNode->GetNeighborSouth() != nullptr && labBlocks[currentNode->GetNeighborSouth()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborSouth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborSouth());

					if (currentNode->GetNeighborEast() != nullptr && labBlocks[currentNode->GetNeighborEast()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborEast()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborEast());

					if (currentNode->GetNeighborWest() != nullptr && labBlocks[currentNode->GetNeighborWest()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborWest()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborWest());

					if (queue.size() == 0) {
						variant = false;
						do {
							currentNode = *(alreadyChecked.begin() + seed.GetUnsignedInt() % alreadyChecked.size());
						} while (std::find(clockPos[i].begin(), clockPos[i].end(), currentNode) != end(clockPos[i]));
						continue;
					}
					else {
						currentNode = queue.back();
						queue.pop_back();
					}
				}
				clockPos[i].push_back(currentNode);// sale, il faudrait utiliser une map container avec key = section.
				currentNode->SetHasClock(true);
			}
		}
		//BELLS
		if (puzzleRoomsType[i] == PuzzleType::Bell) {
			
			ABellPuzzleRoom* bellRoom = nullptr;
			if (HasAuthority()) bellRoom = Cast<ABellPuzzleRoom>(puzzleRooms[i]);

			std::vector<LabBlock*> alreadyChecked;
			LabBlock* currentNode = tilesBeginSection[sectionCounter];
			std::vector<LabBlock*> queue;

			int nbBells = int32(round(float(i) / bandes.size() * 2.f))+2;

			for (int bellId = 0; bellId < nbBells; ++bellId) //TODO replace 4
			{
				float spawnLuck = -0.5;
				bool variant = true;

				while (variant) {
					if (seed.GetFraction() < spawnLuck && !currentNode->GetHasBell())
						variant = false;
					spawnLuck += 0.03;
					alreadyChecked.push_back(currentNode);

					if (currentNode->GetNeighborNorth() != nullptr && labBlocks[currentNode->GetNeighborNorth()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborNorth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborNorth());

					if (currentNode->GetNeighborSouth() != nullptr && labBlocks[currentNode->GetNeighborSouth()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborSouth()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborSouth());

					if (currentNode->GetNeighborEast() != nullptr && labBlocks[currentNode->GetNeighborEast()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborEast()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborEast());

					if (currentNode->GetNeighborWest() != nullptr && labBlocks[currentNode->GetNeighborWest()->GetIndex()].IsLocked() == false
						&& std::find(alreadyChecked.begin(), alreadyChecked.end(), currentNode->GetNeighborWest()) == end(alreadyChecked))
						queue.push_back(currentNode->GetNeighborWest());

					if (queue.size() == 0) {
						variant = false;
						do {
							currentNode = *(alreadyChecked.begin() + seed.GetUnsignedInt() % alreadyChecked.size());
						} while (std::find(bellPos[i].begin(), bellPos[i].end(), currentNode) != end(bellPos[i]));
						continue;
					}
					else {
						currentNode = queue.back();
						queue.pop_back();
					}
				}
				bellPos[i].push_back(currentNode);// sale, il faudrait utiliser une map container avec key = section.
				currentNode->SetHasBell(true);
			}
			do {
				currentNode = *(alreadyChecked.begin() + seed.GetUnsignedInt() % alreadyChecked.size());
			} while (std::find(bellPos[i].begin(), bellPos[i].end(), currentNode) != end(bellPos[i]));
			bellHintPos[i].push_back(currentNode);
		}
		sectionCounter++;
	}
}

void ALabGenerator::CreateStartRoom()
{
	int randomCol = 0;//seed.GetUnsignedInt() % width;
	labBlocks[GetIndex(randomCol, 0)].SetWallNorth(false);

	tilesBeginSection.push_back(&labBlocks[GetIndex(0, 0)]);
	//spawnRoom = GetWorld()->SpawnActor<ASpawnRoom>(ASpawnRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -(0) * LabBlock::assetSize,LabBlock::assetSize ,0 }, FVector{1.f,1.f,1.f}));
	spawnRoom = Cast<ASpawnRoom>(InstanceBP(SpawnRoom_BP/*TEXT("/Game/Blueprints/SpawnRoom_BP.SpawnRoom_BP")*/, FVector{ -(0) * LabBlock::assetSize,LabBlock::assetSize ,0 }, FRotator::ZeroRotator, { 1.f,1.f,1.f }));
	//spawnRoom->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

}

void ALabGenerator::CreatePuzzlesRoom()
{
	int randomColEnd = seed.GetUnsignedInt() % width;
	labBlocks[GetIndex(randomColEnd, height - 1)].SetWallSouth(false);
	int32 saveSeed = seed.GetCurrentSeed();
	int nbPuzzleType = 3;
	std::vector<PuzzleType> puzzleTypes{};
	PuzzleDifficulty difficulties[3] = { PuzzleDifficulty::Easy, PuzzleDifficulty::Medium, PuzzleDifficulty::Hard };

	for (int i = 0; i < bandes.size(); ++i) {
		puzzleTypes.push_back(PuzzleType(i % nbPuzzleType));

		hintClockPos.push_back(std::vector<LabBlock*>{});
		bellPos.push_back(std::vector<LabBlock*>{});
		clockPos.push_back(std::vector<LabBlock*>{});
		lampPos.push_back(std::vector<LabBlock*>{});
		bellHintPos.push_back(std::vector<LabBlock*>{});
	}
	std::for_each(bandes.begin(), bandes.end(),
		[&, counter = 0,last = -1](int bande) mutable {
			int randomCol = seed.GetUnsignedInt() % width;
			int randomPuzzleType;
			do {
				randomPuzzleType = seed.GetUnsignedInt() % FMath::Min(nbPuzzleType - counter % 3, int(puzzleTypes.size()));
			} while (counter % 3 == 0 && puzzleTypes[randomPuzzleType] == last);
			last = puzzleTypes[randomPuzzleType];
			labBlocks[GetIndex(randomCol, bande - 1)].SetWallSouth(false);
			labBlocks[GetIndex(randomCol, bande + 2)].SetWallNorth(false);

			labBlocks[GetIndex(randomCol, bande - 1)].SetNeighborSouth(&(labBlocks[GetIndex(randomCol, bande + 0)]));
			labBlocks[GetIndex(randomCol, bande - 0)].SetNeighborSouth(&(labBlocks[GetIndex(randomCol, bande + 1)]));
			labBlocks[GetIndex(randomCol, bande + 1)].SetNeighborSouth(&(labBlocks[GetIndex(randomCol, bande + 2)]));

			labBlocks[GetIndex(randomCol, bande + 2)].SetNeighborNorth(&(labBlocks[GetIndex(randomCol, bande + 1)]));
			labBlocks[GetIndex(randomCol, bande + 1)].SetNeighborNorth(&(labBlocks[GetIndex(randomCol, bande + 0)]));
			labBlocks[GetIndex(randomCol, bande + 0)].SetNeighborNorth(&(labBlocks[GetIndex(randomCol, bande - 1)]));

			tilesBeginSection.push_back(&labBlocks[GetIndex(randomCol, bande + 2)]);
			//SOLUTION TEMPORAIRE - PLUS SI TEMPORAIRE
			APuzzleRoom* puzzleRoom = nullptr;
			switch (puzzleTypes[randomPuzzleType]) {
			case Clock:
				if (HasAuthority()) {
					puzzleRoom = Cast<AClockPuzzleRoom>(InstanceBP(ClockPuzzleRoom_BP/*TEXT("/Game/Blueprints/ClockPuzzleRoom_BP.ClockPuzzleRoom_BP")*/, FVector{ -randomCol * LabBlock::assetSize, -LabBlock::assetSize * bande, 0 }, FRotator::ZeroRotator, { 1.f,1.f,1.f }));
					puzzleRoom->Tags.Add(FName(FString::FromInt(counter)));
					puzzleRoom->InitPuzzle(seed, difficulties[int32(round(float(counter)/bandes.size()*2.f))]);
				}
				puzzleRoomsType.push_back(PuzzleType::Clock);
				break;
			case Bell:
				if (HasAuthority()) {
					puzzleRoom = Cast<ABellPuzzleRoom>(InstanceBP(BellPuzzleRoom_BP/*TEXT("/Game/Blueprints/BellPuzzleRoom_BP.BellPuzzleRoom_BP")*/, FVector{ -randomCol * LabBlock::assetSize, -LabBlock::assetSize * bande, 0 }, FRotator::ZeroRotator, { 1.f,1.f,1.f }));
					puzzleRoom->Tags.Add(FName(FString::FromInt(counter)));
					int32 diff = int32(round(float(counter) / bandes.size() * 2.f));
					puzzleRoom->InitPuzzle(seed, difficulties[diff]);
				}
				puzzleRoomsType.push_back(PuzzleType::Bell);
				break;
			case Lamp:
				if (HasAuthority()) {
					puzzleRoom = Cast<ALampPuzzleRoom>(InstanceBP(LampPuzzleRoom_BP/*TEXT("/Game/Blueprints/LampPuzzleRoom_BP.LampPuzzleRoom_BP")*/, FVector{ -randomCol * LabBlock::assetSize, -LabBlock::assetSize * bande, 0 }, FRotator::ZeroRotator, { 1.f,1.f,1.f }));
					puzzleRoom->Tags.Add(FName(FString::FromInt(counter)));
					puzzleRoom->InitPuzzle(seed, difficulties[int32(round(float(counter) / bandes.size() * 2.f))]);
				}
				puzzleRoomsType.push_back(PuzzleType::Lamp);
				break;
			}
			if (puzzleRoom)
			{
				puzzleRooms.Add(puzzleRoom);
				puzzleRoom->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);
			}
			puzzleTypes.erase(puzzleTypes.begin() + randomPuzzleType);
			counter++;
			//
		});

	//Update LabBlocks Info Section SubSection
	int sectionId = 0;
	for (LabBlock* labBlock : tilesBeginSection) {
		int subSectionId = 0;
		std::vector<LabBlock*> queue;
		std::vector<LabBlock*> bin;
		queue.push_back(labBlock);
		LabBlock* itBlock = labBlock;
		while (!queue.empty()) {
			itBlock = queue.back();
			queue.pop_back();
			if (std::find(bin.begin(), bin.end(), itBlock) == bin.end())
				bin.push_back(itBlock);
			itBlock->SetSectionId(sectionId);
			if (itBlock->GetNeighborNorth() != nullptr && !itBlock->GetNeighborNorth()->IsLocked()
				&& std::find(bin.begin(), bin.end(), itBlock->GetNeighborNorth()) == bin.end()
				&& std::find(queue.begin(), queue.end(), itBlock->GetNeighborNorth()) == queue.end())
				queue.push_back(itBlock->GetNeighborNorth());
			if (itBlock->GetNeighborEast() != nullptr && !itBlock->GetNeighborEast()->IsLocked()
				&& std::find(bin.begin(), bin.end(), itBlock->GetNeighborEast()) == bin.end()
				&& std::find(queue.begin(), queue.end(), itBlock->GetNeighborEast()) == queue.end())
				queue.push_back(itBlock->GetNeighborEast());
			if (itBlock->GetNeighborSouth() != nullptr && !itBlock->GetNeighborSouth()->IsLocked()
				&& std::find(bin.begin(), bin.end(), itBlock->GetNeighborSouth()) == bin.end()
				&& std::find(queue.begin(), queue.end(), itBlock->GetNeighborSouth()) == queue.end())
				queue.push_back(itBlock->GetNeighborSouth());
			if (itBlock->GetNeighborWest() != nullptr && !itBlock->GetNeighborWest()->IsLocked()
				&& std::find(bin.begin(), bin.end(), itBlock->GetNeighborWest()) == bin.end()
				&& std::find(queue.begin(), queue.end(), itBlock->GetNeighborWest()) == queue.end())
				queue.push_back(itBlock->GetNeighborWest());
		}
		++sectionId;
	}

	//Make last section like long forward tunnel.
	LabBlock* curr = tilesBeginSection.back();
	for (int i = 0; i < subSectionSize; ++i) {
		curr->SetWallsToVoid();
		curr->SetWallSouth(false);
		curr->SetWallNorth(false);
		if( i != subSectionSize-1)
			curr = &labBlocks[GetIndex(curr->GetX(),curr->GetY()+1)];
	}
	AActor * endRoom = InstanceBP(EndPuzzleRoom_BP/*TEXT("/Game/Blueprints/EndPuzzleRoom_BP.EndPuzzleRoom_BP")*/, FVector{ -curr->GetX() * LabBlock::assetSize ,-curr->GetY() * LabBlock::assetSize,0 }, FRotator::ZeroRotator, { 1.f,1.f,1.f });

	// TO Remove : add a puzzleRoom at the end.
	//APuzzleRoom* puzzleRoomEnd = GetWorld()->SpawnActor<APuzzleRoom>(APuzzleRoom::StaticClass(), FTransform(FQuat::Identity, FVector{ -randomColEnd * LabBlock::assetSize  , -LabBlock::assetSize * height, 0 }, FVector{ 1.f, 1.f, 1.f }));

	//puzzleRooms.Add(puzzleRoomEnd);
	//puzzleRoomEnd->AttachToActor(this, FAttachmentTransformRules::KeepWorldTransform);

	seed.Initialize(FName(FString::FromInt(saveSeed)));

}

float ALabGenerator::GetCellInfluenceAtPos(FVector absPos) {
	int x = round(absPos.X / -LabBlock::assetSize);
	int y = round(absPos.Y / -LabBlock::assetSize);

	return (x >= 0 && y >= 0) ? tiles[GetIndex(x, y)]->inf_final : 1.0f;
}