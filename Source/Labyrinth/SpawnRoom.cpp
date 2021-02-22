// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnRoom.h"
#include "GameFramework/PlayerStart.h"
#include "LabCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "LabyrinthGameModeBase.h"

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

		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn0"), FRotator::ZeroRotator, SpawnInfo);
		GetWorld()->GetAuthGameMode()->SpawnDefaultPawnAtTransform();
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn1"), FRotator::ZeroRotator, SpawnInfo);
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn2"), FRotator::ZeroRotator, SpawnInfo);
		GetWorld()->SpawnActor<APlayerStart>(APlayerStart::StaticClass(), mesh->GetSocketLocation("Spawn3"), FRotator::ZeroRotator, SpawnInfo);
	
	}
}	
