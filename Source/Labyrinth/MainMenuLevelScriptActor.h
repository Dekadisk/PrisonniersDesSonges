#pragma once

#include "CoreMinimal.h"
#include "Engine/LevelScriptActor.h"
#include "MainMenuLevelScriptActor.generated.h"

UCLASS()
class LABYRINTH_API AMainMenuLevelScriptActor : public ALevelScriptActor
{
	GENERATED_BODY()

protected:

	virtual void BeginPlay() override;
	
};
