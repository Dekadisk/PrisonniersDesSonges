// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "KeyPickUpActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AKeyPickUpActor : public APickUpActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* KeyMesh;
public:
	AKeyPickUpActor();
	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;
	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;
};
