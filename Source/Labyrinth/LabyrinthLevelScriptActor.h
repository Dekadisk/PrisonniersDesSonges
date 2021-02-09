// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "LabyrinthLevelScriptActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALabyrinthLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
};
