#include "PuzzleRoom.h"
#include "Engine/StaticMeshSocket.h"
#include "MushroomDecorator.h"
#include "RockDecorator.h"

APuzzleRoom::APuzzleRoom() {

	bReplicates = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSpawnRoom(TEXT("StaticMesh'/Game/Assets/Cave/CaveRoom.CaveRoom'"));

	if (MeshSpawnRoom.Succeeded()) {
		spawnRoom = MeshSpawnRoom.Object;
		width = 2;
		height = 2;
	}

	mesh->SetStaticMesh(spawnRoom);

	//DECORATIONS
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshRock(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Rocks/SM_LCave_P_Free_Rock_03.SM_LCave_P_Free_Rock_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshMite(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mite_Tites/SM_LCave_P_Free_MiteTite_02.SM_LCave_P_Free_MiteTite_02'"));
	
	if (MeshRock.Succeeded())
		Rock = MeshRock.Object;
	if (MeshMite.Succeeded())
		Mite = MeshMite.Object;
}

void APuzzleRoom::BeginPlay()
{
	Super::BeginPlay();

	//DECORATIONS
	FRandomStream seed;
	seed.Initialize("SalutMonPote");
	TArray<FName> socketNames = mesh->GetAllSocketNames();
	for (FName socketName : socketNames) {

		if (socketName.ToString().Contains("Mushroom")) {

			const UStaticMeshSocket* socket = mesh->GetSocketByName(socketName);
			FTransform transform;
			socket->GetSocketTransform(transform, mesh);

			AMushroomDecorator* mushroom = Cast<AMushroomDecorator>(InstanceBP(TEXT("/Game/Blueprints/Mushroom_BP.Mushroom_BP")
				, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
			mushroom->setKind(seed.GetUnsignedInt());
		}

		if (socketName.ToString().Contains("LittleRock")) {
			const UStaticMeshSocket* socket = mesh->GetSocketByName(socketName);
			FTransform transform;
			socket->GetSocketTransform(transform, mesh);

			ARockDecorator* rock = Cast<ARockDecorator>(InstanceBP(TEXT("/Game/Blueprints/Rock_BP.Rock_BP")
				, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
			rock->setKind(seed.GetUnsignedInt());

		}
	}
}

void APuzzleRoom::InitPuzzle(FRandomStream seed, PuzzleDifficulty _difficulty)
{
	difficulty = _difficulty;
	const UStaticMeshSocket* socketChalkOnChair = mesh->GetSocketByName("ChalkOnChair0");
	FTransform transformChalkOnChair;
	socketChalkOnChair->GetSocketTransform(transformChalkOnChair, mesh);
	AActor* ChalkOnChair = InstanceBP(TEXT("/Game/Blueprints/ChalkOnChair_BP.ChalkOnChair_BP"), transformChalkOnChair.GetLocation(), transformChalkOnChair.GetRotation().Rotator(), transformChalkOnChair.GetScale3D());

}

AActor* APuzzleRoom::InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale)
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

