// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Cachette.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ACachette : public AUsableActor
{
	GENERATED_BODY()

public:
	ACachette();

public:

	// Meshes
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PorteG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PorteD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* TiroirG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* TiroirD;

	// Useful booleans
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Status")
	bool bIsOpen;

	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr);

	UFUNCTION(BlueprintImplementableEvent)
	void SlightlyOpen();

	UFUNCTION(BlueprintImplementableEvent)
		void AllOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void Close();
};
