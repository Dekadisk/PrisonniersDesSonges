// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Cachette.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ACachette : public AUsableActor
{
	GENERATED_BODY()

public:
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr);
};
