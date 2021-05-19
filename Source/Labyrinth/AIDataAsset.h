#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIDataAsset.generated.h"

UCLASS()
class LABYRINTH_API UAIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	// AI Controller
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sensing")
		bool Sight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Navigation")
		float NavRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pacing")
		float ThreateningDist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Pacing")
		float MinHuntTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tasks")
		float PlayerNearCachetteDist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tasks")
		float DistToPuzzle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tasks")
		float LoseFocus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tasks")
		float GainFocus;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Tasks")
		float DistToAttack;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		int Level;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		int LevelMax;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		int BeingTrapped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		int FoundInCachette;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		float ChancesToDestroyCachettePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		float ChancesToDestroyCachettePerFound;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		float ChancesToDestroyObstaclePerLevel;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Evolution")
		float ChancesToDestroyObstaclePerTrapped;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float BasePatrolSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float BaseChaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float BaseWanderSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float MaxPatrolSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float MaxChaseSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Movement")
		float MaxWanderSpeed;

	// AI Director
	UPROPERTY(EditAnywhere, Category = "Pacing")
		float stopWandering;

	UPROPERTY(EditAnywhere, Category = "Pacing")
		float stopPatrolling;

	UPROPERTY(EditAnywhere, Category = "Pacing")
		float AngerTime;
};
