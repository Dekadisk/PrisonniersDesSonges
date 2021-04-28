// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "InfluenceDataAsset.generated.h"

/**
 * 
 */
UENUM()
enum class InfluenceGroup {
	Solvable,
	Pickup,
	Puzzle,
	HidingSpot,
	DeadEnd,
	Trap,
	Players,
	Hints
};

UENUM()
enum class BlendModes {
	Additive,
	AlphaAdditive
};

UCLASS()
class LABYRINTH_API UInfluenceDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	InfluenceGroup influenceGroup;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float influence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float alternativeInfluence;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	BlendModes blendMode;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float blendAlpha;


		
};
