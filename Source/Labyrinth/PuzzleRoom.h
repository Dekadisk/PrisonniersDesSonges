// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Room.h"
#include "PuzzleRoom.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API APuzzleRoom : public ARoom
{
	GENERATED_BODY()
public:
	/*    CaveRoomStart     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* spawnRoom;

public:
	APuzzleRoom();

	void BeginPlay() override;
	
};
