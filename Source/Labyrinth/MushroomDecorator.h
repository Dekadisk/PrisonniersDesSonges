// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Decorator.h"
#include "MushroomDecorator.generated.h"

UCLASS()
class LABYRINTH_API AMushroomDecorator : public ADecorator
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMushroomDecorator();

	/**    Mushroom M1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M1;
	/**    Mushroom M2     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M2;
	/**    Mushroom M3     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M3;
	/**    Mushroom M4     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M4;
	/**    Mushroom M5     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M5;
	/**    Mushroom M6     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M6;
	/**    Mushroom M7     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M7;
	/**    Mushroom M8     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M8;
	/**    Mushroom M9     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
	class UStaticMesh* M9;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void UpdateMesh();

	virtual void OnRep_UpdateMesh();

	virtual int getKind() { return kind%9; }

	virtual void setKind(int _kind) { kind = _kind % 9; UpdateMesh(); }
};
