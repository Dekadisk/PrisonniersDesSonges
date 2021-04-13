// Fill out your copyright notice in the Description page of Project Settings.


#include "Cachette.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"

void ACachette::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* MyPawn = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(MyPawn))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(MyPawn->GetController());
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