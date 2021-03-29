// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CheckPuzzlesBTService.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UCheckPuzzlesBTService : public UBTService
{
	GENERATED_BODY()
	
public:
	UCheckPuzzlesBTService();

	/** Sera appel� � chaque �update� du service */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
};
