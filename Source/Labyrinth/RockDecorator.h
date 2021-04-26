// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RockDecorator.generated.h"

UCLASS()
class LABYRINTH_API ARockDecorator : public AActor
	// Fill out your copyright notice in the Description page of Project Settings.
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARockDecorator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshs");
	UStaticMeshComponent* mesh;

	/**    Rock R1     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* R1;
	/**    Rock R2     */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Static Meshs")
		class UStaticMesh* R2;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite/*, ReplicatedUsing = OnRep_UpdateMesh*/);
	/// There are 2 kind of rocks :
	int kind;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		void UpdateMesh();

	UFUNCTION()
		void OnRep_UpdateMesh();

	UFUNCTION()
		int getKind() { return kind % 2; }

	UFUNCTION()
		void setKind(int _kind) { kind = _kind % 2; UpdateMesh(); }
};
