#include "SpawnRoom.h"

ASpawnRoom::ASpawnRoom() {

	bReplicates = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSpawnRoom(TEXT("StaticMesh'/Game/Assets/Cave/CaveRoomStart.CaveRoomStart'"));
	
	if (MeshSpawnRoom.Succeeded())
		spawnRoom = MeshSpawnRoom.Object;

	mesh->SetStaticMesh(spawnRoom);
	mesh->SetRelativeRotation(FQuat({ 0,0,1 }, PI));
}

void ASpawnRoom::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.bNoFail = false;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = NULL;
		SpawnInfo.bDeferConstruction = false;
	}
}	
