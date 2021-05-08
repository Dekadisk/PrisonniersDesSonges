#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "TrapPickUpActor.generated.h"

/**
 * UNUSED CLASS. TO REMOVE !
 */
UCLASS()
class LABYRINTH_API ATrapPickUpActor : public APickUpActor
{
	GENERATED_BODY()

public:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* TrapMesh;

public:

	ATrapPickUpActor();

	UPROPERTY()
	bool bIsOpen;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arr�te de regarder l'objet
	virtual void OnEndFocus() override;

	void Use(bool Event, APawn* InstigatorPawn = nullptr) override;
};
