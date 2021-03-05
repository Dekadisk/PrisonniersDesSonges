// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIDirector.generated.h"

UCLASS()
class LABYRINTH_API AAIDirector : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AAIDirector();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPlayer(AActor* actor);

	void UpdateThreats(float DeltaTime);

	AActor* NextPlayerTarget();

	float GenerateThreat(AActor* player);

	void DirectMonster();
	
	void DebugDisplayInfo();

private:

	const float tickRate = 1.0f;

	class AAIEnemyController * Monster;

	TArray<AActor*> Players;

	TArray<AActor*> Solvables;

	TMap<AActor*, float> Threats;

	float CalculateMeanDistToPlayers();

};
