// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoom.h"
#include "GameFramework/PlayerStart.h"
#include "LabCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LabyrinthGameModeBase.h"
//
APuzzleRoom::APuzzleRoom() {
	bReplicates = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSpawnRoom(TEXT("StaticMesh'/Game/Assets/Cave/CaveRoom.CaveRoom'"));
	if (MeshSpawnRoom.Succeeded()) {
		spawnRoom = MeshSpawnRoom.Object;
		width = 2;
		height = 2;
	}

	mesh->SetStaticMesh(spawnRoom);




}

void APuzzleRoom::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{

	}
}

void APuzzleRoom::InitPuzzle(FRandomStream seed)
{

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
