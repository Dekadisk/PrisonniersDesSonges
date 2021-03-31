#include "SpawnRoom.h"
#include "GameFramework/PlayerStart.h"

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

		
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn0"), mesh->GetSocketRotation("Spawn0"), SpawnInfo);
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn1"), mesh->GetSocketRotation("Spawn1"), SpawnInfo);
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn2"), mesh->GetSocketRotation("Spawn2"), SpawnInfo);
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn3"), mesh->GetSocketRotation("Spawn3"), SpawnInfo);
	}
}	
