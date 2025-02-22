#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Components/BoxComponent.h"
#include "PlatePuzzleActor.generated.h"

UCLASS()
class LABYRINTH_API APlatePuzzleActor : public AUsableActor
{
	GENERATED_BODY()

public:

	APlatePuzzleActor();

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* Plate;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UBoxComponent* OverlapBox;

	UPROPERTY(VisibleAnywhere)
	bool bIsPressed = false;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "1", ClampMax = "4", UIMin = "1", UIMax = "4"))
	int32 minChar;

	float minWeight;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Animation
	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	// Appel� quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
};
