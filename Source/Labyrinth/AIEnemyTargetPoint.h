// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/TargetPoint.h"
#include "AIEnemyTargetPoint.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AAIEnemyTargetPoint : public ATargetPoint
{
	GENERATED_BODY()

public:
	AAIEnemyTargetPoint();

	/* Ordre de patrouille du monstre */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TargetPoint")
	int32 Position;
};
