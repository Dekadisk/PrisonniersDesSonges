// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Components/BoxComponent.h"
#include "TriggerBoxActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ATriggerBoxActor : public AUsableActor
{
	GENERATED_BODY()
	
public:
	ATriggerBoxActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* OverlapBox;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
