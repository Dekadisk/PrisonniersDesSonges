#pragma once

#include "UsableActor.h"
#include "LeverPuzzleActor.generated.h"

UCLASS()
class LABYRINTH_API ALeverPuzzleActor : public AUsableActor
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditDefaultsOnly, Category = "Mesh")
	UStaticMeshComponent* MeshLeverBase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshLeverStick;

	UPROPERTY(VisibleAnywhere, Transient, ReplicatedUsing = OnRep_EnableDisableAnimation)
	bool isEnable;

	ALeverPuzzleActor();

	// Le joueur regarde l'objet
	virtual void OnBeginFocus() override;

	// Le joueur arrête de regarder l'objet
	virtual void OnEndFocus() override;

	// Appelé quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	//UFUNCTION(Reliable, Server, WithValidation)
	//void ServerOnUsed(AActor* InstigatorActor);

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	//Animation Blueprint
	UFUNCTION(BlueprintImplementableEvent)
	void EnableAnimation();

	UFUNCTION(BlueprintImplementableEvent)
	void DisableAnimation();

	UFUNCTION()
	void OnRep_EnableDisableAnimation();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere, Transient, Replicated)
	bool isProcessing;

	int GetEtat() override;

};
