// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
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

	/** Sera utilisé par la tâche UpdateNextTargetPointBTTaskNode du
	 Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void UpdateNextTargetPoint();

	/**
	* Nous vérifions si le personnage est près et si c'est le cas, nous plaçons
	* une référence dans le BlackBoard. Cette fonction sera utilisée par le service
	* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
	* nous approchons de sa zone de patrouille.
	*/
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void CheckNearbyEnemy();

	/* Même objectif, avec un rayon. */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void CheckNearbyEnemyRay();

	/**
	* Fait en sorte que notre AIEnemyCharacter poursuive le joueur référé par la clé
	 * TargetActorToFollow du Blackboard. Sera utilisée dans une tâche du BT pour suivre le personnage
	 * principal (le joueur). En sortie, nous retournerons le résultat de MoveToActor (Failed,
	 * AlreadyAtGoal ou RequestSuccessful)
	*/
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		EPathFollowingRequestResult::Type MoveToEnemy();

};
