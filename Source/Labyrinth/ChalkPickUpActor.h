// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "ChalkPickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AChalkPickUpActor : public APickUpActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* ChalkMesh;
public:
	AChalkPickUpActor();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	void Use(bool Event, APawn* InstigatorPawn = nullptr) override;
};
