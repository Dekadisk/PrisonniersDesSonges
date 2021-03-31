#pragma once

#include "CoreMinimal.h"
#include "LabCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LABYRINTH_API APlayerCharacter : public ALabCharacter
{
	GENERATED_BODY()

public:
	
	void BeginPlay() override;
};
