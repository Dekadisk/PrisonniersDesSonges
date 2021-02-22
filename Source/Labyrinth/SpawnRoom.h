// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Room.h"
#include "SpawnRoom.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ASpawnRoom : public ARoom
{
	GENERATED_BODY()
public:
	/**    CaveRoomStart     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* spawnRoom;

public:
	ASpawnRoom();

	void BeginPlay() override;
};
