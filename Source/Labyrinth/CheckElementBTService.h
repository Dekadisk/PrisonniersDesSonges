// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckElementBTService.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UCheckElementBTService : public UBTService
{
	GENERATED_BODY()

public:
	UCheckElementBTService();

	/** Sera appelé à chaque «update» du service */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
	
};
