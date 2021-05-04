// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Decorator.h"
#include "RabbitDecorator.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ARabbitDecorator : public ADecorator
{
	GENERATED_BODY()

protected:
	/**    Mesh M1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* M1;
	/**    Mesh M2     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* M2;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	ARabbitDecorator();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateMesh()override;

	virtual void OnRep_UpdateMesh() override;

	virtual int getKind() override { return kind % 2; }

	virtual void setKind(int _kind) override { kind = _kind % 2; UpdateMesh(); }
};
