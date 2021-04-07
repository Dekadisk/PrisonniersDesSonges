// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LabCharacter.h"
#include "LanternHeld.generated.h"


UCLASS(Blueprintable)
class LABYRINTH_API ALanternHeld : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALanternHeld();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Le mesh de l'arme: vue � la 3i�me personne */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
		UStaticMeshComponent* Mesh;

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void OnEnterInventory(ALabCharacter* NewOwner);
	void OnLeaveInventory();

	void SetOwningPawn(ALabCharacter* NewOwner);

	/** Owner */
	UPROPERTY(Transient, ReplicatedUsing = OnRep_MyPawn)
		ALabCharacter* MyPawn;

	/* Associer l'arme � une position d'inventaire*/
	void AttachMeshToPawn(FName slot);
	/* Nous d�tachons le mesh de l'acteur - operation 3d */
	void DetachMeshFromPawn();
	/** Obtenir le mesh de l'arme */
	UFUNCTION(BlueprintCallable, Category = "Game|Weapon")
		UStaticMeshComponent* GetLanternMesh() const;
	UFUNCTION()
		void OnRep_MyPawn();
	
	/*
	virtual void OnLeaveInventory();
	bool IsEquippedOrPending() const;
	virtual void OnUnEquip();

	// ******* �quipement
    // D�part de l'�quipement
	float EquipStartedTime;
	// Dur�e de l'�quipement
	float EquipDuration;
	bool bIsEquipped;
	bool bPendingEquip;

	// Animations et son
	UPROPERTY(EditDefaultsOnly)
		UAnimMontage* EquipAnim;
	UPROPERTY(EditDefaultsOnly, Category = "Sounds")
		USoundCue* EquipSound;
	// �quipement
	void OnEquip();
	bool IsEquipped() const;
	virtual void OnEquipFinished();
	float GetEquipStartedTime() const;
	float GetEquipDuration() const;*/

};
