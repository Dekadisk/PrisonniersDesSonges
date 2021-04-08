#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "ProceduralLevelScriptActor.generated.h"

UCLASS()
class LABYRINTH_API AProceduralLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()
	
public:

	void BeginPlay() override;
};
