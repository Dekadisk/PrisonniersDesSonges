// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleActor.h"
#include "ButtonPuzzleActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AButtonPuzzleActor : public APuzzleActor
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* MeshButtonBase;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* MeshButtonStick;

	AButtonPuzzleActor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;
	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;
	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor) override;

	//UFUNCTION(Reliable, Server, WithValidation)
	//void ServerOnUsed(AActor* InstigatorActor);

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent)
		void ActivateAnimation();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Replicated)
	bool isProcessing;
};
