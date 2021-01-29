// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleActor.h"
#include "ClockPuzzleActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AClockPuzzleActor : public APuzzleActor
{
	GENERATED_BODY()

public:

	AClockPuzzleActor();

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ClockBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ClockCenter;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "7", UIMin = "0", UIMax = "7"))
	int32 startPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "7", UIMin = "0", UIMax = "7"))
	int32 unlockPos;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 currPos;
	int32 maxPos;
	bool isAlreadyCalledAction;
	bool isProcessing;
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;
	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;
	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor) override;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	// Animation
	UFUNCTION(BlueprintImplementableEvent)
	void Rotate();

	virtual void OnConstruction(const FTransform& Transform) override;
	
};
