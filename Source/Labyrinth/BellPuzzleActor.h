// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PuzzleActor.h"
#include "Sound/SoundCue.h"
#include "BellPuzzleActor.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API ABellPuzzleActor : public APuzzleActor
{
	GENERATED_BODY()
	
public:
	ABellPuzzleActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* Bell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BellStick;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int32 note;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* NoteSound;

	bool isProcessing;

	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor) override;

	virtual void BeginPlay() override;
	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;
	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
