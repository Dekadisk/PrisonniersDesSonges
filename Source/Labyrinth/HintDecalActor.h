// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DecalActor.h"
#include "HintDecalActor.generated.h"

UENUM()
enum class TypeHint
{
	CLOCK_POS0,
	CLOCK_POS1,
	CLOCK_POS2,
	CLOCK_POS3,
	CLOCK_POS4,
	CLOCK_POS5,
	CLOCK_POS6,
	CLOCK_POS7,
	ECLOCK_POS0,
	ECLOCK_POS1,
	ECLOCK_POS2,
	ECLOCK_POS3,
	ECLOCK_POS4,
	ECLOCK_POS5,
	ECLOCK_POS6,
	ECLOCK_POS7
};

static const unsigned int decalErase = 8;


UCLASS()
class LABYRINTH_API AHintDecalActor : public ADecalActor
{
	GENERATED_BODY()

public:
	AHintDecalActor();

public:

	UFUNCTION()
	void OnRep_UpdateMaterial();

	UFUNCTION()
	void Erase();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_UpdateMaterial);
	TypeHint kind = TypeHint::CLOCK_POS0;

	UPROPERTY(VisibleAnywhere)
	bool isErased;

	/** Decal material. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Decal)
	UMaterialInterface* erased;

	UPROPERTY(EditAnywhere)
	class APuzzleActor* puzzleActor;


private:
	UPROPERTY()
	TArray<UMaterial*> matHints;
};
