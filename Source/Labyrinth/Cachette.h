// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "LabyrinthPlayerController.h"
#include "LabCharacter.h"
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
		UStaticMeshComponent* Hitboite;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PorteG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PorteD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* TiroirG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* TiroirD;

	// Array of players in Cachette
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InCupboardPlayers")
		TArray<ALabyrinthPlayerController*> InCupboardPlayers;

	// Useful booleans
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsProcessing;

	// Functions
	//
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr);

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Custom Events
	//
	UFUNCTION(BlueprintImplementableEvent)
	void SlightlyOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void AllOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void Close();

	UFUNCTION()
	void OnBeginFocus();

	UFUNCTION()
	void OnEndFocus();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastOpen();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastClose();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastRemovePlayer(ALabyrinthPlayerController* playerController);

	virtual void Tick(float a);
};
