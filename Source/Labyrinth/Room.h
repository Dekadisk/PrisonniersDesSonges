#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Room.generated.h"

UCLASS()
class LABYRINTH_API ARoom : public AActor
{
	GENERATED_BODY()

public:	

	// Sets default values for this actor's properties
	ARoom();

	unsigned int height, width;

	UStaticMeshComponent * mesh;

};
