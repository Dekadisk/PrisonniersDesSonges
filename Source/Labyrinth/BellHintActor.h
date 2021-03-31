#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Sound/SoundCue.h"
#include "BellHintActor.generated.h"

UCLASS()
class LABYRINTH_API ABellHintActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	ABellHintActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* HintMesh;

	UAudioComponent* AudioComponent;

	UPROPERTY(EditAnywhere, Category = "Puzzle")
	TArray<int32> waited;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<USoundCue*> NotesSounds;

	int lastPlayed = 0;

	bool isProcessing;

	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	UFUNCTION()
	void NextNote();

	void BeginPlay() override;

	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor) override;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	virtual void OnConstruction(const FTransform& Transform) override;
};
