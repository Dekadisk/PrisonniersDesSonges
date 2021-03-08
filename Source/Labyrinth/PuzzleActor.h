// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "PuzzleTools.h"
#include "HintDecalActor.h"
#include "PuzzleActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API APuzzleActor : public AUsableActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	APuzzleActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UPROPERTY(EditAnywhere)
	TArray<FLinkedActors> targetActor;

	UPROPERTY(EditAnywhere)
	TSubclassOf<AHintDecalActor> hintClass = nullptr;

	UPROPERTY(EditAnywhere)
	AHintDecalActor* visualHint;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;
	// Le joueur arr�te de regarder l'objet
	virtual void OnEndFocus() override;
	// Appel� quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor) override;

	virtual void ProcessTargetActions(bool b);

	//Multi
	virtual void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

};
