// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "LabCharacter.generated.h"

UCLASS()
class LABYRINTH_API ALabCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ALabCharacter();

public:
	/** Vitesse de déplacement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement")
	float Vitesse;

	/** Touche Run Active */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement")
	bool bPressedRun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement", Transient, Replicated)
	bool bNotSeenYet;

	// Distance maximale de focus sur les objets.
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	// Seulement vrai lors de la première image avec un nouveau focus.


	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	bool bHasNewFocus;

	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	class AUsableActor* FocusedUsableActor;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
		void Forward(float Value);

	UFUNCTION()
		void Right(float Value);

	//On active le booléen bPressedJump
	UFUNCTION()
		void OnStartJump();
	// On désactive le booléen bPressedJump
	UFUNCTION()
		void OnStopJump();

	//On active le booléen bPressedRun
	UFUNCTION(BlueprintCallable, Category = "Mouvement")
		bool IsRunning();

	//On active la course
	UFUNCTION()
		void OnStartRun();
	// On désactive la course
	UFUNCTION()
		void OnStopRun();

	UFUNCTION()
		void Use();
	UFUNCTION(BlueprintCallable, Category = "Status")
		class AUsableActor* GetUsableInView();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
