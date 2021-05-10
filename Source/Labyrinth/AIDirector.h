 #pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AIDirector.generated.h"

UCLASS()
class LABYRINTH_API AAIDirector : public AActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	AAIDirector();

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void AddPlayer(AActor* actor);

	void UpdateThreats(float DeltaTime);

	AActor* NextTarget();

	float GenerateThreat(AActor* player);

	void VerifyPlayersAlive();

	void DirectMonster();
	
	void DebugDisplayInfo();

private:

	float timeWandering = 0.0f;

	float timePatrolling = 0.0f;

	class AAIEnemyController * Monster;

	UPROPERTY()
	TArray<AActor*> Players;

	UPROPERTY()
	TArray<AActor*> Solvables;

	UPROPERTY()
	TMap<AActor*, float> Threats;

	float CalculateMeanDistToPlayers();

};
