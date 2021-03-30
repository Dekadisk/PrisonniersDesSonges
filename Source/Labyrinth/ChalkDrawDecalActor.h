// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "Engine/DecalActor.h"
#include "ChalkDrawDecalActor.generated.h"

/**
 * 
 */

UENUM()
enum class TypeDraw
{
	FRONT_ARROW,
	LEFT_ARROW,
	RIGHT_ARROW,
	QUESTION_MARK,
	CROSS,
	CIRCLE
};

UCLASS()
class LABYRINTH_API AChalkDrawDecalActor : public ADecalActor
{
	GENERATED_BODY()
	
public:

	AChalkDrawDecalActor();

public:

	UFUNCTION()
	void OnRep_UpdateMaterial();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_UpdateMaterial);
	TypeDraw kind = TypeDraw::FRONT_ARROW;

private:
	UPROPERTY()
	UMaterial* CircleMaterial;

	UPROPERTY()
	UMaterial* CrossMaterial;

	UPROPERTY()
	UMaterial* InterrogationMaterial;

	UPROPERTY()
	UMaterial* LeftArrowMaterial;

	UPROPERTY()
	UMaterial* RightArrowMaterial;

	UPROPERTY()
	UMaterial* FrontArrowMaterial;
	

};
