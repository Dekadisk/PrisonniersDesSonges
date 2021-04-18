// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "PuzzleEventsData.h"
#include "EventMaker.generated.h"

UCLASS()
class LABYRINTH_API AEventMaker : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEventMaker();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ExecuteEvent(FPE_PuzzleEvent pe);

	UFUNCTION()
		void SpawnActors(FPE_PuzzleEvent pe);
	UFUNCTION()
		void SpawnActor(FPE_ActorSpawn e);
	UFUNCTION()
		void TransformActors(FPE_PuzzleEvent pe);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void TransformActor(FPE_ActorTransformations Event);
	UFUNCTION()
		void InteractActors(FPE_PuzzleEvent pe);
	UFUNCTION()
		void InteractActor(FPE_ActorInteractions e);
	UFUNCTION()
	void HideActors(FPE_PuzzleEvent pe);
	UFUNCTION()
	void HideActor(FPE_ActorHiding e);
	UFUNCTION(Reliable, NetMulticast)
	void MulticastDisableCollision(bool disable, AActor* a);

	UFUNCTION()
	void LookAtActors(FPE_PuzzleEvent pe);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void LookAtActor(FPE_LookAtActors Event);

	UFUNCTION(NetMulticast,Reliable)
	void MulticastLookAtActor(FPE_LookAtActors Event);


	UFUNCTION()
	void UpdateObjectives(FPE_PuzzleEvent pe);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void UpdateObjective(FPE_UpdateObjective Event);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateObjective(FPE_UpdateObjective Event);

	UFUNCTION()
		void UpdateSubtitles(FPE_PuzzleEvent pe);
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
		void UpdateSubtitle(FPE_SubtitleSeq Event);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastUpdateSubtitle(FPE_SubtitleSeq Event);
};
