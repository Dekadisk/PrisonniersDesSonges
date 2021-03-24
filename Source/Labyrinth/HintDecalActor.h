// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "HintDecalActor.generated.h"


UCLASS()
class LABYRINTH_API AHintDecalActor : public ADecalActor
{
	GENERATED_BODY()

public:
	AHintDecalActor();
	
public: 
	UPROPERTY(EditAnywhere)
	class APuzzleActor* puzzleActor;

	UPROPERTY(VisibleAnywhere)
	bool isErased;

	/** Decal material. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Decal)
	UMaterialInterface* erased;

	virtual void Erase();
};
