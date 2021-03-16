// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include <map>
#include "Perception/AIPerceptionComponent.h"
#include "AIEnemyController.generated.h"

/**
 * 
 */
UCLASS()
class LABYRINTH_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AAIEnemyController();

	const float ThreateningDist = 1500.0f;

	const float SightRadius = 3000.0f;

	//UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Awareness)
	//UAIPerceptionComponent* PerceptionComponent;
	
	/** Sera utilisé par la tâche UpdateNextTargetPointBTTaskNode du
	 Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void UpdateNextTargetPoint();

	UFUNCTION(BlueprintCallable, Category = "AIPerception")
	void Sensing(const TArray<AActor*>& actors);

	/**
	* Nous vérifions si le personnage est près et si c'est le cas, nous plaçons
	* une référence dans le BlackBoard. Cette fonction sera utilisée par le service
	* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
	* nous approchons de sa zone de patrouille.
	*/

	/* Même objectif, avec un rayon. */
	/*UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void CheckNearbyEnemy();*/

	/**
	* Fait en sorte que notre AIEnemyCharacter poursuive le joueur référé par la clé
	 * TargetActorToFollow du Blackboard. Sera utilisée dans une tâche du BT pour suivre le personnage
	 * principal (le joueur). En sortie, nous retournerons le résultat de MoveToActor (Failed,
	 * AlreadyAtGoal ou RequestSuccessful)
	*/
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		EPathFollowingRequestResult::Type MoveToEnemy();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void CheckElementChangedState();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// SENSING
	//void SenseHearing();
	//void SenseSight();

	class AAIEnemyTargetPoint* PreviousTargetPoint;

	TArray<class APuzzleActor*> PuzzlesInMemory;

};
