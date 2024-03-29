#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "Sound/SoundCue.h"
#include "AkAudioEvent.h"
#include "BellPuzzleActor.generated.h"

UCLASS()
class LABYRINTH_API ABellPuzzleActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	ABellPuzzleActor();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Shell;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* Pendulum;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ArmL;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* ArmR;

	UPROPERTY(EditAnywhere, meta = (ClampMin = "0", ClampMax = "6", UIMin = "0", UIMax = "6"), Replicated)
	int32 note;

	UPROPERTY(EditAnywhere, Category = "Sound")
	TArray<UAkAudioEvent*> WNoteSounds;

	bool isProcessing;

	UFUNCTION(BlueprintImplementableEvent)
	void Animate();

	UFUNCTION(NetMulticast, Reliable)
	void NetMulticastAnimate(APawn* InstigatorPawn);

	// Appel� quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arr�te de regarder l'objet
	virtual void OnEndFocus() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void UpdateScale();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
