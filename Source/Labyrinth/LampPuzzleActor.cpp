// Fill out your copyright notice in the Description page of Project Settings.


#include "LampPuzzleActor.h"
#include "SolvableActor.h"
#include "AkGameplayStatics.h"

ALampPuzzleActor::ALampPuzzleActor() {
	Socle = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Socle"));
	Abat = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Abat"));
	Tiges = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tiges"));
	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));

	Socle->AttachTo(MeshComp);
	Abat->AttachTo(MeshComp);
	Tiges->AttachTo(MeshComp);
	Light->AttachTo(MeshComp);


	static ConstructorHelpers::FObjectFinder<UMaterial> FMOff(TEXT("/Game/Assets/Usable/Puzzle/Lamp/eb_lamp_01.eb_lamp_01"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FMOn(TEXT("/Game/Assets/Usable/Puzzle/Lamp/eb_lamp_01ON.eb_lamp_01ON"));
	if (FMOff.Succeeded()) OffMat = FMOff.Object;
	if (FMOn.Succeeded()) OnMat = FMOn.Object;
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

void ALampPuzzleActor::OnRep_UseLamp()
{
	Light->SetHiddenInGame(!isOn);
	if (isOn) Abat->SetMaterial(0,OnMat);
	else Abat->SetMaterial(0,OffMat);

}

void ALampPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALampPuzzleActor, isOn);
}

void ALampPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
		// Utilise par notre PostProcess pour le rendu d'un surlignage
		Abat->SetRenderCustomDepth(true);
		Socle->SetRenderCustomDepth(true);
}

void ALampPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
		// Utilise par notre PostProcess pour le rendu d'un surlignage
		Abat->SetRenderCustomDepth(false);
		Socle->SetRenderCustomDepth(false);
}

void ALampPuzzleActor::Use(bool Event, APawn* InstigatorPawn) {

	UAkGameplayStatics::PostEvent(SonBouton, this, 0, FOnAkPostEventCallback::FOnAkPostEventCallback());

	isOn = !isOn;
	Light->SetHiddenInGame(!isOn);
	if (isOn) Abat->SetMaterial(0, OnMat);
	else Abat->SetMaterial(0, OffMat);

	if (!Event)
		CheckEvents(EPuzzleEventCheck::Use);
	if (isOn)
		CheckEvents(EPuzzleEventCheck::On);
	else
		CheckEvents(EPuzzleEventCheck::Off);
}

int ALampPuzzleActor::GetEtat() {
	TArray<ALampPuzzleActor*> adjLamps;
	adjLamps.Add(this);
	for (auto pem : PuzzleEvents) {
		for (auto pei : pem.Event.Environment.ActorInteractions) {
			if (pei.Interaction == EPuzzleEventInteraction::Use) {
				for (auto a : pei.Actors) {
					if (ALampPuzzleActor* lamp = Cast<ALampPuzzleActor>(a)) {
						adjLamps.Add(lamp);
					}
				}				
			}
		}
	}

	int lampsOn = 0;
	for (auto l : adjLamps) {
		if (l->isOn) {
			lampsOn++;
		}
	}

	if (lampsOn == adjLamps.Num() && isOn)
		return -1;
	else
		return lampsOn;

	
}