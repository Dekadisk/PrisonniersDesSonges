// Fill out your copyright notice in the Description page of Project Settings.


#include "LampPuzzleActor.h"
#include "SolvableActor.h"


ALampPuzzleActor::ALampPuzzleActor() {
	Socle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Socle"));
	Abat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Abat"));
	Tiges = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tiges"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));

	Socle->AttachTo(MeshComp);
	Abat->AttachTo(MeshComp);
	Tiges->AttachTo(MeshComp);
	Light->AttachTo(MeshComp);
}

void ALampPuzzleActor::OnConstruction(const FTransform& Transform)
{
	Light->SetHiddenInGame(!isOn);
}

void ALampPuzzleActor::BeginPlay() {
	Super::BeginPlay();
	if (HasAuthority() && isOn) {
		for (FPE_PuzzleEventMaster& pem : PuzzleEvents) {
			if (pem.Trigger == EPuzzleEventCheck::On) {
				for (FPE_ActorInteractions& ai : pem.Event.Environment.ActorInteractions) {
					for (AActor* a : ai.Actors) {
						if (ASolvableActor* sa = Cast<ASolvableActor>(a)) {
							if (ai.Interaction == EPuzzleEventInteraction::Unlock)
								sa->Unlock(true);
							else if(ai.Interaction == EPuzzleEventInteraction::Lock)
								sa->Lock(true);
						}
					}
				}
			}
		}
	}	
}

void ALampPuzzleActor::Use(bool Event, APawn* InstigatorPawn) {
	isOn = !isOn;
	Light->SetHiddenInGame(!isOn);
	if (!Event)
		CheckEvents(EPuzzleEventCheck::Use);
	if (isOn)
		CheckEvents(EPuzzleEventCheck::On);
	else
		CheckEvents(EPuzzleEventCheck::Off);
}