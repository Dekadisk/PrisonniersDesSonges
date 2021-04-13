// Fill out your copyright notice in the Description page of Project Settings.


#include "Cachette.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

ACachette::ACachette() {

	PorteG = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PorteG_MESH"));
	PorteG->SetupAttachment(MeshComp);

	PorteD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PorteD_MESH"));
	PorteD->SetupAttachment(MeshComp);

	TiroirG = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TiroirG_MESH"));
	TiroirG->SetupAttachment(MeshComp);

	TiroirD = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TiroirD_MESH"));
	TiroirD->SetupAttachment(MeshComp);

	bIsOpen = false;
}

void ACachette::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* MyPawn = Cast<ALabCharacter>(InstigatorPawn);
	ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(MyPawn->GetController());
	if (MyPawn->bIsHidden) {
		if (bIsOpen) {
			Close();
			bIsOpen = false;
		}
		else {
			SlightlyOpen();
			bIsOpen = true;
		}
	}
	else {
		if (bIsOpen) {
			Close();
			bIsOpen = false;
		}
		else {
			AllOpen();
			bIsOpen = true;
		}
	}

	if (IsValid(MyPawn))
	{
		playerController = Cast<ALabyrinthPlayerController>(MyPawn->GetController());
		if (!MyPawn->bIsHidden) {
			Super::Use(Event, InstigatorPawn);
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("IgnoreOnlyPawn"), true);
			MyPawn->OnHide();
		}
		else {
			MyPawn->OnUnhide();
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("IgnoreOnlyPawn"), false);
			MeshComp->UPrimitiveComponent::SetCollisionProfileName(FName("Default"), true);
		}
	}
}