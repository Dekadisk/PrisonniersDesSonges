// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Decorator.generated.h"

UCLASS()
class LABYRINTH_API ADecorator : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADecorator();


public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Meshs");
	UStaticMeshComponent* mesh;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite/*, ReplicatedUsing = OnRep_UpdateMesh*/);
	/// There are some kind of meshes :
	int kind;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
		virtual void UpdateMesh() {};

	UFUNCTION()
		virtual void OnRep_UpdateMesh() {};

	UFUNCTION()
		virtual int getKind() { return 0; };

	UFUNCTION()
		virtual void setKind(int _kind) { kind = _kind; };

};
