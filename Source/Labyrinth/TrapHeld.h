// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabCharacter.h"
#include "TrapHeld.generated.h"

UCLASS()
class LABYRINTH_API ATrapHeld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATrapHeld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Le mesh de l'arme: vue à la 3ième personne */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Mesh)
		UStaticMeshComponent* Mesh;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnEnterInventory(ALabCharacter* NewOwner);
	void OnLeaveInventory();

	void SetOwningPawn(ALabCharacter* NewOwner);

	/** Owner */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		ALabCharacter* MyPawn;

	/* Associer l'arme à une position d'inventaire*/
	void AttachMeshToPawn(FName slot);
	/* Nous détachons le mesh de l'acteur - operation 3d */
	void DetachMeshFromPawn();
	/** Obtenir le mesh de l'arme */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
		UStaticMeshComponent* GetLanternMesh() const;
	UFUNCTION()
		void OnRep_MyPawn();

	UFUNCTION(BlueprintImplementableEvent, Category = "Lantern")
		void ActivatePhysics();

};
