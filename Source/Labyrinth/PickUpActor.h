// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "UsableActor.h"
#include "Sound/SoundCue.h"
#include "kismet/GameplayStatics.h"
#include "Net/UnrealNetwork.h"
#include "PickupActor.generated.h"

UCLASS()
class LABYRINTH_API APickUpActor : public AUsableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APickUpActor();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
		USoundCue* PickupSound;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void OnUsed(AActor* InstigatorActor) override;

};
