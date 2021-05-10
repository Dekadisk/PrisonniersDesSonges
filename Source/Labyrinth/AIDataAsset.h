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

	// AI Director
	UPROPERTY(EditAnywhere, Category = "Pacing")
		float stopWandering;

	UPROPERTY(EditAnywhere, Category = "Pacing")
		float stopPatrolling;
};
