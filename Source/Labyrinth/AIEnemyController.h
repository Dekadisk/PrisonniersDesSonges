// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include <map>
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

	/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du
	 Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void UpdateNextTargetPoint();

	/**
	* Nous v�rifions si le personnage est pr�s et si c'est le cas, nous pla�ons
	* une r�f�rence dans le BlackBoard. Cette fonction sera utilis�e par le service
	* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
	* nous approchons de sa zone de patrouille.
	*/
	/*UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void CheckNearbyEnemy();*/

	/* M�me objectif, avec un rayon. */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void CheckNearbyEnemyRay();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void UpdateLastSeen(float DeltaSeconds);

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		void ResetLastSeen();

	/**
	* Fait en sorte que notre AIEnemyCharacter poursuive le joueur r�f�r� par la cl�
	 * TargetActorToFollow du Blackboard. Sera utilis�e dans une t�che du BT pour suivre le personnage
	 * principal (le joueur). En sortie, nous retournerons le r�sultat de MoveToActor (Failed,
	 * AlreadyAtGoal ou RequestSuccessful)
	*/
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		EPathFollowingRequestResult::Type MoveToEnemy();

private:

	TArray<APlayerState*> GetClosestCharacters();

	std::map<uint32, float> PlayersLastSeen;

	ACharacter* PlayerToFollow;

	void ResetPlayerLastSeen(uint32 id);
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

};
