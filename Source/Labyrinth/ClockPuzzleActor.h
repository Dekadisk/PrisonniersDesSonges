#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "ClockPuzzleActor.generated.h"

UCLASS()
class LABYRINTH_API AClockPuzzleActor : public AUsableActor
{
	GENERATED_BODY()

public:

	AClockPuzzleActor();

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* ClockBase;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ClockCenter;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "7", UIMin = "0", UIMax = "7"))
	int32 startPos;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ClampMin = "0", ClampMax = "7", UIMin = "0", UIMax = "7"))
	int32 unlockPos;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	int32 currPos;

	int32 maxPos;

	bool isAlreadyCalledAction = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool isProcessing;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arr�te de regarder l'objet
	virtual void OnEndFocus() override;

	// Appel� quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;
	
	// Animation
	UFUNCTION(BlueprintImplementableEvent)
	void Rotate();

	virtual void OnConstruction(const FTransform& Transform) override;

	int GetEtat() override;
	
};
