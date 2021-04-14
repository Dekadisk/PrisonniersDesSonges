// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EdGraph/EdGraphNode.h"
#include "InfluenceMapNode.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API UInfluenceMapNode : public UEdGraphNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY()
	FVector globalPos;

	UPROPERTY()
	TMap<FString, float> values;

public:
	UFUNCTION()
	void SetGlobalPos(FVector _globalPos) { globalPos = _globalPos; }
	UFUNCTION()
	FVector GetGlobalPos() { return globalPos; }

	UFUNCTION()
	TMap<FString, float>& GetValues() { return values; }




};
