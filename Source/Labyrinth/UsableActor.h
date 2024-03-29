#pragma once

#include "Core.h"
#include "GameFramework/Actor.h"
#include "Net/UnrealNetwork.h"
#include "PuzzleEventsData.h"
#include "EventMaker.h"
#include "InfluencerActor.h"
#include "UsableActor.generated.h"

UCLASS()
class LABYRINTH_API AUsableActor : public AInfluencerActor
{
	GENERATED_BODY()

private:

	TSubclassOf<AEventMaker> EventMaker;

public:

	// Sets default values for this actor's properties
	AUsableActor();

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Replicated)
	bool bDisableFocus;

	UPROPERTY(EditAnywhere, Category = "Events")
	TArray<FPE_PuzzleEventMaster> PuzzleEvents;

public:

	virtual void BeginPlay() override;

	// Le joueur regarde l'objet
	virtual void OnBeginFocus();

	// Le joueur arr�te de regarder l'objet
	virtual void OnEndFocus();

	// Appel� quand le joueur interagit avec l'objet
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr) {
	}

	virtual void AlternativeUse(bool Event, APawn* InstigatorPawn = nullptr) {
		CheckEvents(EPuzzleEventCheck::AlternativeUse);
	}

	virtual void Unlock(bool Event) {}
	virtual void Lock(bool Event) {}
	virtual void On(bool Event) {}
	virtual void Off(bool Event) {}
	virtual void Ring(bool Event, int32 note) {}
	virtual void Activate(bool Event) {}

	UFUNCTION(BlueprintCallable)
	virtual void CheckEvents(EPuzzleEventCheck check, APawn* InstigatorPawn = nullptr);

	virtual int GetEtat() { return 0; }

	//Multi
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

};
