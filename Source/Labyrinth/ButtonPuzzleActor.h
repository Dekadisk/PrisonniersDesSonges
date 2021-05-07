// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "ButtonPuzzleActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AButtonPuzzleActor : public AUsableActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MeshButtonBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshButtonMovablePart;

	AButtonPuzzleActor();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	// Appelé quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void UseAnimation();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastUseAnimation();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool isProcessing;
};
