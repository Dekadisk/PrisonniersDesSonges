// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InfluenceDataAsset.h"
#include "InfluencerActor.generated.h"

UCLASS()
class LABYRINTH_API AInfluencerActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AInfluencerActor();

	UPROPERTY(EditAnywhere, Category = "InfluenceMap")
	UInfluenceDataAsset * InfluenceDataAsset;

	UFUNCTION()
	virtual bool shouldUseAlternativeInfluence() { return false; }; // overridable

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
