#include "PuzzleRoom.h"
#include "Engine/StaticMeshSocket.h"
#include "RockDecorator.h"
#include "Misc/DefaultValueHelper.h"

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

	//DECORATIONS
	//ROCKS
	int counter = 0;
	TArray<FName> socketNames = mesh->GetAllSocketNames();
	for (FName name : socketNames) {
		if (name.ToString().Contains("Rock")) {
			UStaticMeshComponent* rock = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Rock") + FString::FromInt(counter++)));
			rock->SetupAttachment(mesh, name);
			rock->SetStaticMesh(Rock);
		}
		if (name.ToString().Contains("Mite")) {
			UStaticMeshComponent* mite = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Mite") + FString::FromInt(counter++)));
			mite->SetupAttachment(mesh, name);
			mite->SetStaticMesh(Mite);
		}
	}
	// BLUEPRINTS
	static ConstructorHelpers::FClassFinder<AActor> ChalkOnChair_BP_F(TEXT("/Game/Blueprints/ChalkOnChair_BP"));
	static ConstructorHelpers::FClassFinder<ALanternPickUpActor> LanternPickUpActor_BP_F(TEXT("/Game/Blueprints/LanternPickUpActor_BP"));
	static ConstructorHelpers::FClassFinder<AMushroomDecorator> Mushroom_BP_F(TEXT("/Game/Blueprints/Mushroom_BP"));
	static ConstructorHelpers::FClassFinder<ARockDecorator> Rock_BP_F(TEXT("/Game/Blueprints/Rock_BP"));
	static ConstructorHelpers::FClassFinder<AActor> FireTorch_BP_F(TEXT("/Game/CustomMaterials/FireTorch_BP"));
	ChalkOnChair_BP = ChalkOnChair_BP_F.Class;
	LanternPickUpActor_BP = LanternPickUpActor_BP_F.Class;
	Mushroom_BP = Mushroom_BP_F.Class;
	Rock_BP = Rock_BP_F.Class;
	FireTorch_BP = FireTorch_BP_F.Class;
}

void APuzzleRoom::BeginPlay()
{
	Super::BeginPlay();

	//DECORATIONS

	//return; // DESACTIVATION TEMPORAIRE
	FRandomStream seed;
	seed.Initialize("SalutMonPote");
	TArray<FName> socketNames = mesh->GetAllSocketNames();
	for (FName socketName : socketNames) {

		if (socketName.ToString().Contains("Mushroom")) {

			const UStaticMeshSocket* socket = mesh->GetSocketByName(socketName);
			FTransform transform;
			socket->GetSocketTransform(transform, mesh);

			AMushroomDecorator* mushroom = Cast<AMushroomDecorator>(InstanceBP(Mushroom_BP/*TEXT("/Game/Blueprints/Mushroom_BP.Mushroom_BP")*/
				, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
			mushroom->setKind(seed.GetUnsignedInt());
			decorationsTagDynamic.Add(mushroom);
			socketName.ToString().Contains("MushroomB") ?
				decorationsAreBegin.Add(true) : decorationsAreBegin.Add(false);
		}

		if (socketName.ToString().Contains("LittleRock")) {
			const UStaticMeshSocket* socket = mesh->GetSocketByName(socketName);
			FTransform transform;
			socket->GetSocketTransform(transform, mesh);

			ARockDecorator* rock = Cast<ARockDecorator>(InstanceBP(Rock_BP/*TEXT("/Game/Blueprints/Rock_BP.Rock_BP")*/
				, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
			rock->setKind(seed.GetUnsignedInt());

		}

		if (socketName.ToString().Contains("Torch")) {
			const UStaticMeshSocket* socket = mesh->GetSocketByName(socketName);
			FTransform transform;
			socket->GetSocketTransform(transform, mesh);

			AActor* torch = InstanceBP(FireTorch_BP/*TEXT("/Game/CustomMaterials/FireTorch_BP.FireTorch_BP")*/
				, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());

			decorationsTagDynamic.Add(torch);
			socketName.ToString().Contains("TorchB") ?
				decorationsAreBegin.Add(true) : decorationsAreBegin.Add(false);

		}
	}
}

void APuzzleRoom::InitPuzzle(FRandomStream seed, PuzzleDifficulty _difficulty)
{
	difficulty = _difficulty;
	const UStaticMeshSocket* socketChalkOnChair = mesh->GetSocketByName("ChalkOnChair0");
	const UStaticMeshSocket* socketLantern = mesh->GetSocketByName("Lantern0");
	FTransform transformChalkOnChair;
	FTransform transformLantern;
	socketChalkOnChair->GetSocketTransform(transformChalkOnChair, mesh);
	socketLantern->GetSocketTransform(transformLantern, mesh);
	AActor* ChalkOnChair = InstanceBP(ChalkOnChair_BP/*TEXT("/Game/Blueprints/ChalkOnChair_BP.ChalkOnChair_BP")*/, transformChalkOnChair.GetLocation(), transformChalkOnChair.GetRotation().Rotator(), transformChalkOnChair.GetScale3D());
	AActor* lantern = InstanceBP(LanternPickUpActor_BP/*TEXT("/Game/Blueprints/LanternPickUpActor_BP.LanternPickUpActor_BP")*/, transformLantern.GetLocation(), transformLantern.GetRotation().Rotator(), transformLantern.GetScale3D());

	for (int i = 0; i < decorationsTagDynamic.Num();++i) {
		if (decorationsAreBegin[i] && Tags.Num() > 0)
			decorationsTagDynamic[i]->Tags.Add(Tags[0]);
		else if (!decorationsAreBegin[i] && Tags.Num() > 0) {
			int32 tag_number;
			FDefaultValueHelper::ParseInt(Tags[0].ToString(), tag_number);
			decorationsTagDynamic[i]->Tags.Add(FName(FString::FromInt(tag_number + 1)));
		}
	}

}

AActor* APuzzleRoom::InstanceBP(TSubclassOf<UObject> blueprint, FVector location, FRotator rotation, FVector scale)
{
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

