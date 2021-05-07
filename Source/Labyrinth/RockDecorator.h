// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Decorator.h"
#include "RockDecorator.generated.h"

UCLASS()
class LABYRINTH_API ARockDecorator : public ADecorator
	// Fill out your copyright notice in the Description page of Project Settings.
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockDecorator();

	/**    Rock R1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* R1;
	/**    Rock R2     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* R2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	virtual void UpdateMesh()override;

	virtual void OnRep_UpdateMesh() override;

	virtual int getKind() override { return kind % 2; }

	virtual void setKind(int _kind) override { kind = _kind % 2; UpdateMesh(); }
};
