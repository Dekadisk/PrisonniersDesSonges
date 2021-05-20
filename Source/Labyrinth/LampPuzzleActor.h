// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Components/PointLightComponent.h"
#include "AkAudioEvent.h"
#include "LampPuzzleActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ALampPuzzleActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	ALampPuzzleActor();

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* Socle;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* Abat;

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* Tiges;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UPointLightComponent* Light;

	UPROPERTY(EditAnywhere, Category = "Lamp", ReplicatedUsing = OnRep_UseLamp)
	bool isOn;

	UPROPERTY()
	UMaterial * OffMat;
	
	UPROPERTY()
	UMaterial * OnMat;

	UPROPERTY(EditDefaultsOnly, Category = "WWise")
	UAkAudioEvent* SonBouton;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrete de regarder l'objet
	virtual void OnEndFocus() override;

	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void BeginPlay() override;
	
	UFUNCTION()
	void OnRep_UseLamp();

	virtual int GetEtat() override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
