// Fill out your copyright notice in the Description page of Project Settings.


#include "ClockPuzzleActor.h"
#include <algorithm>


AClockPuzzleActor::AClockPuzzleActor() {
	ClockBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBase"));
	ClockCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockCenter"));

	ClockBase->AttachTo(MeshComp);
	ClockCenter->AttachTo(MeshComp);

	maxPos = 7;

}

void AClockPuzzleActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AClockPuzzleActor::OnBeginFocus()
{
	Super::OnBeginFocus();

	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		ClockCenter->SetRenderCustomDepth(true);
	}
}

void AClockPuzzleActor::OnEndFocus()
{
	Super::OnEndFocus();

	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		ClockCenter->SetRenderCustomDepth(false);
	}
}

void AClockPuzzleActor::OnUsed(AActor* InstigatorActor)
{
	if (!isProcessing)
	{
		Rotate();
		currPos = (currPos + 1) % (maxPos + 1);
		if (targetActor.Num() > 0)
		{
			if (currPos == unlockPos) {
				ProcessTargetActions(true);
				isAlreadyCalledAction = false;
			}
			else {
				if (!isAlreadyCalledAction) {
					ProcessTargetActions(false);
					isAlreadyCalledAction = true;
				}
			}
		}
	}
}

void AClockPuzzleActor::BeginPlay()
{
	Super::BeginPlay();
}


void AClockPuzzleActor::OnConstruction(const FTransform& Transform)
{
	FRotator NewRotator = FRotator::ZeroRotator; // starts with everything as 0.0f
	NewRotator.Roll = startPos * -45.0f; // new value of 10.0f
	ClockCenter->SetRelativeRotation(NewRotator);
	currPos = startPos;
}

int AClockPuzzleActor::GetEtat() {
	if (currPos == unlockPos)
		return -1;
	return currPos;
}