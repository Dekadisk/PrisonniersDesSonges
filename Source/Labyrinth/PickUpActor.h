#pragma once

#include "Core.h"
#include "UsableActor.h"
#include "Sound/SoundCue.h"
#include "AkAudioEvent.h"
#include "PickupActor.generated.h"

UCLASS()
class LABYRINTH_API APickUpActor : public AUsableActor
{
	GENERATED_BODY()

public:

	// Sets default values for this actor's properties
	APickUpActor();

	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	UAkAudioEvent* PickupSound;
	
	UPROPERTY(BlueprintReadWrite)
	FVector savePos;

public:

	void Use(bool Event, APawn* InstigatorPawn = nullptr) override;
};
