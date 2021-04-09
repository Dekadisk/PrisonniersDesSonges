#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "AIController.h"
#include <map>
#include "Perception/AIPerceptionComponent.h"
#include "AIEnemyController.generated.h"

UCLASS()
class LABYRINTH_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:
	AAIEnemyController();

	const float ThreateningDist = 1500.0f;

	const float SightRadius = 3000.0f;
	
	/** Sera utilis� par la t�che UpdateNextTargetPointBTTaskNode du
	 Behavior Tree pour actualiser le chemin de patrouille */
	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void UpdateNextTargetPoint();

	UFUNCTION(BlueprintCallable, Category = "AIPerception")
	void Sensing(const TArray<AActor*>& actors);

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
	EPathFollowingRequestResult::Type ChangeZone();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void ClearBlackboard();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void UsePuzzle();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void FindPlayerToAttack();

private:

	class AAIEnemyTargetPoint* PreviousTargetPoint;

	TMap<class AUsableActor*, int> PuzzlesInMemory;

	int currentSection = 0;

};
