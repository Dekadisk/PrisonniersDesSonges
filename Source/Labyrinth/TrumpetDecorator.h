// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Decorator.h"
#include "TrumpetDecorator.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ATrumpetDecorator : public ADecorator
{
	GENERATED_BODY()

protected:
	/**    Mesh R1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M1;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ATrumpetDecorator();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateMesh()override;

	virtual void OnRep_UpdateMesh() override;

	virtual int getKind() override { return kind % 1; }

	virtual void setKind(int _kind) override { kind = _kind % 1; UpdateMesh(); }

};
