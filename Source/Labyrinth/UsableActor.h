// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "UsableActor.generated.h"

UCLASS()
class LABYRINTH_API AUsableActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AUsableActor();

	UPROPERTY(BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Transient, Replicated)
	bool bDisableFocus;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Le joueur regarde l'objet
	virtual void OnBeginFocus();
	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus();
	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor);

	//Multi
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
