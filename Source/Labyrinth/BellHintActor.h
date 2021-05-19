#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "AkAudioEvent.h"
#include "BellHintActor.generated.h"

UCLASS()
class LABYRINTH_API ABellHintActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	ABellHintActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* HintMesh;

	UPROPERTY(EditAnywhere, Category = "Puzzle", Replicated)
	TArray<int32> waited;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<UAkAudioEvent*> WNotesSounds;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<UAkAudioEvent*> WNotesSamples;

	int lastPlayed = 0;

	bool isProcessing;

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastAnimate(APawn* InstigatorPawn);

	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	UFUNCTION()
	void NextNote();

	// Appelé quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
