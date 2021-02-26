// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PuzzleTools.h"
#include "SolvableActor.generated.h"

UCLASS(ABSTRACT)
class LABYRINTH_API ASolvableActor : public AStaticMeshActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASolvableActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool isSolved;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool hasErasedHint = false;

	UPROPERTY(EditAnywhere)
	TArray<FPuzzleElemsNeeded> needed;

	TArray<class APuzzleActor*> elements;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void Unlock() PURE_VIRTUAL(ASolvableActor::Unlock, );
	virtual void Lock() PURE_VIRTUAL(ASolvableActor::Lock, );
	virtual void Activate() PURE_VIRTUAL(ASolvableActor::Activate, );
	virtual void Open() PURE_VIRTUAL(ASolvableActor::Open, );
	virtual void Close() PURE_VIRTUAL(ASolvableActor::Close, );

};
