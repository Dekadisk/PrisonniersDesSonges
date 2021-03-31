#pragma once

#include "PuzzleActor.h"
#include "LeverPuzzleActor.generated.h"

UCLASS()
class LABYRINTH_API ALeverPuzzleActor : public APuzzleActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MeshLeverBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshLeverStick;

	UPROPERTY(VisibleAnywhere, Transient, Replicated)
	bool isEnable;

	ALeverPuzzleActor();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	// Appelé quand le joueur interagit avec l'objet
	virtual void OnUsed(AActor* InstigatorActor) override;

	//UFUNCTION(Reliable, Server, WithValidation)
	//void ServerOnUsed(AActor* InstigatorActor);

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void EnableAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableAnimation();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Replicated)
	bool isProcessing;

	int GetEtat() override;

};
