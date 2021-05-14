#pragma once

#include "Core.h"
#include "AIController.h"
#include "Perception/AISenseConfig_Sight.h"
#include "AIDataAsset.h"
#include "AIEnemyController.generated.h"

UCLASS()
class LABYRINTH_API AAIEnemyController : public AAIController
{
	GENERATED_BODY()

public:

	AAIEnemyController();

	UPROPERTY(EditAnywhere, Category = "DataAsset")
	UAIDataAsset* DataAsset;

	const float SightRadius = 3000.0f;

	UPROPERTY(EditAnywhere, Category = "Sight")
	UAISenseConfig_Sight* sightConfig;
	
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
	EPathFollowingRequestResult::Type MoveToPlayer();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	EPathFollowingRequestResult::Type ChangeZone();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void ClearBlackboard();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void UsePuzzle();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void AttackPlayer();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void StartHunt();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void DestroyCachette();

	UFUNCTION(BlueprintCallable, Category = "AIEnemyController")
	void DestroyTrap();

	int currentSection = 0;

private:

	class AAIEnemyTargetPoint* PreviousTargetPoint;

	TMap<class AUsableActor*, int> PuzzlesInMemory;

	TArray<AActor*> ElementsInSight;

	void PlayerSeen(AActor* player);

	void PredictPlayerMvmt(AActor* PlayerActor);

};
