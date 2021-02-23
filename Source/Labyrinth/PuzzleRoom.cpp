// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzleRoom.h"
#include "GameFramework/PlayerStart.h"
#include "LabCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LabyrinthGameModeBase.h"

APuzzleRoom::APuzzleRoom() {
	bReplicates = true;
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshSpawnRoom(TEXT("StaticMesh'/Game/Assets/Cave/CaveRoomStart.CaveRoomStart'"));
	if (MeshSpawnRoom.Succeeded())
		spawnRoom = MeshSpawnRoom.Object;

	mesh->SetStaticMesh(spawnRoom);




}

void APuzzleRoom::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{

	}
}