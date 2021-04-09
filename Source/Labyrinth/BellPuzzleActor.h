#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Sound/SoundCue.h"
#include "BellPuzzleActor.generated.h"

UCLASS()
class LABYRINTH_API ABellPuzzleActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	ABellPuzzleActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	USkeletalMeshComponent* Bell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* BellStick;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"))
	int32 note;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundWave* NoteSound;

	bool isProcessing;

	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	// Appel� quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	virtual void BeginPlay() override;
	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;
	// Le joueur arr�te de regarder l'objet
	virtual void OnEndFocus() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	void UpdateScale();
};
