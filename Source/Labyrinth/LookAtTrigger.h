// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Components/BoxComponent.h"
#include "PlayerCharacter.h"
#include "LookAtTrigger.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALookAtTrigger : public AUsableActor
{
	GENERATED_BODY()
	
public:
	ALookAtTrigger();

	void Looking(APlayerCharacter*);
	void ResetLooking(APlayerCharacter*);

	UPROPERTY(EditAnywhere, Category = "Settings")
	float lookingDuration;

	UPROPERTY(EditAnywhere, Category = "Settings")
	float maxDist;

	UPROPERTY(EditAnywhere, Category = "Collision")
	UBoxComponent* OverlapBox;
private:
	TMap<APlayerCharacter*, float> timers;
	TArray<APlayerCharacter*> alreadyCalled;
};
