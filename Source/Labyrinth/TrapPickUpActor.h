// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "TrapPickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ATrapPickUpActor : public APickUpActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* TrapMesh;
public:
	ATrapPickUpActor();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	void Use(bool Event, APawn* InstigatorPawn = nullptr) override;
};
