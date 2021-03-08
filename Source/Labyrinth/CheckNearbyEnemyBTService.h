// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckNearbyEnemyBTService.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UCheckNearbyEnemyBTService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckNearbyEnemyBTService();

	/** Sera appelé à chaque «update» du service */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
};

