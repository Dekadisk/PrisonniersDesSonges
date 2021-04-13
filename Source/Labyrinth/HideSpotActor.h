#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "HideSpotActor.generated.h"

UCLASS()
class LABYRINTH_API AHideSpotActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	AActor* hiddenActor;

public:

	void Use(bool Event, APawn* InstigatorPawn) override;
};
