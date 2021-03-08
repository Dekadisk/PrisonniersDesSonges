// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorActor.h"
#include "LabCharacter.h"

ADoorActor::ADoorActor() {
	bIsLocked = true;
	bIsOpen = false;
	//fOpenPercent = 0.f;
	//fSpeedOpen = 2;//percent
}

void ADoorActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	ALabCharacter* LabCharacter = Cast<ALabCharacter>(InstigatorActor);
	if (LabCharacter && bIsLocked) {

		if (LabCharacter->bHasKey) {
			LabCharacter->bHasKey = false;
			bIsLocked = false;
			bIsOpen = true;
			OpenAnimation();
		}
		else {
			GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, TEXT("Cannot unlock door. (no key found)"));
		}
	}
	else {
		bIsOpen = !bIsOpen;
		bIsOpen ? OpenAnimation() : CloseAnimation();
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Yellow, TEXT("Toggle door"));
	}
}

void ADoorActor::Tick(float DeltaTime)
{
	/*if (bIsOpen && !isgreaterequal(fOpenPercent,100.f)) {
		fOpenPercent += fSpeedOpen;
		AddActorLocalRotation(FQuat({ 0,0,1 }, PI/2 * fSpeedOpen/100));
	}
	else if(!bIsOpen && !islessequal(fOpenPercent,0.f)) {
		fOpenPercent -= fSpeedOpen;
		AddActorLocalRotation(FQuat({ 0,0,1 }, -PI / 2 * fSpeedOpen/100));
	}*/
}

void ADoorActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ADoorActor, bIsLocked);
	DOREPLIFETIME(ADoorActor, bIsOpen);
}
