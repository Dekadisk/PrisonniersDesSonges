// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "HideSpotActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AHideSpotActor : public AUsableActor
{
	GENERATED_BODY()
	
public:
	AHideSpotActor();

	AActor* hiddenActor;
public:
	void OnUsed(AActor* InstigatorActor) override;
};
