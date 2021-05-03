// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Decorator.h"
#include "FrameDecorator.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AFrameDecorator : public ADecorator
{
	GENERATED_BODY()

protected:
	/**    Mesh M1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* M1;
	/**    Mesh M2     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* M2;
	/**    Mesh M3     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* M3;
	/**    Mesh M4     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* M4;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	AFrameDecorator();
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateMesh()override;

	virtual void OnRep_UpdateMesh() override;

	virtual int getKind() override { return kind % 4; }

	virtual void setKind(int _kind) override { kind = _kind % 4; UpdateMesh(); }
};
