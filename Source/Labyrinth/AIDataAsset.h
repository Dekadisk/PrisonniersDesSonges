#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "AIDataAsset.generated.h"

UCLASS()
class LABYRINTH_API UAIDataAsset : public UDataAsset
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool Sight;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float ThreateningDist;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float NavRadius;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		float MinHuntTime;
};
