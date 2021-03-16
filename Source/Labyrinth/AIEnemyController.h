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
	
	/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du
	 Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void UpdateNextTargetPoint();

	UFUNCTION(BlueprintCallable, Category = "AIPerception")
	void Sensing(const TArray<AActor*>& actors);

	/**
	* Nous v�rifions si le personnage est pr�s et si c'est le cas, nous pla�ons
	* une r�f�rence dans le BlackBoard. Cette fonction sera utilis�e par le service
	* CheckNearbyEnemyBTService du BT pour implanter la vigilance du NPC lorsque
	* nous approchons de sa zone de patrouille.
	*/

	/* M�me objectif, avec un rayon. */
	/*UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void CheckNearbyEnemy();*/

	/**
	* Fait en sorte que notre AIEnemyCharacter poursuive le joueur r�f�r� par la cl�
	 * TargetActorToFollow du Blackboard. Sera utilis�e dans une t�che du BT pour suivre le personnage
	 * principal (le joueur). En sortie, nous retournerons le r�sultat de MoveToActor (Failed,
	 * AlreadyAtGoal ou RequestSuccessful)
	*/
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
		EPathFollowingRequestResult::Type MoveToEnemy();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void CheckElementChangedState(AActor* actor);

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void CheckPuzzlesToInvestigate();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void UpdateFocus();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void Wander();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	EPathFollowingRequestResult::Type MoveToPriorityPoint();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void ClearBlackboard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:

	// SENSING
	//void SenseHearing();
	//void SenseSight();

	class AAIEnemyTargetPoint* PreviousTargetPoint;

	TMap<class APuzzleActor*, int> PuzzlesInMemory;

};
