#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "LanternHeld.h"
#include "LanternPickUpActor.generated.h"

UCLASS()
class LABYRINTH_API ALanternPickUpActor : public APickUpActor
{

	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
		UStaticMeshComponent* LanternMesh;

	UPROPERTY()
	TSubclassOf<ALanternHeld> LanternHeld_BP;
		
public:

	ALanternPickUpActor();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	AActor* SpawnHeld_BP();

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

};
