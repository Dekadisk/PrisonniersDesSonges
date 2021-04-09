#include "ClockPuzzleActor.h"

AClockPuzzleActor::AClockPuzzleActor() {
	ClockBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBase"));
	ClockCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockCenter"));

	ClockBase->AttachTo(MeshComp);
	ClockCenter->AttachTo(MeshComp);

	maxPos = 7;
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

void AClockPuzzleActor::Use(bool Event, APawn* InstigatorPawn)
{
	if (!isProcessing)
	{
		Rotate();
		currPos = (currPos + 1) % (maxPos + 1);
		if (currPos == unlockPos) {
			CheckEvents(EPuzzleEventCheck::Unlock, InstigatorPawn);
			isAlreadyCalledAction = false;
		}
		else {
			if (!isAlreadyCalledAction) {
				CheckEvents(EPuzzleEventCheck::Lock, InstigatorPawn);
				isAlreadyCalledAction = true;
			}
		}
	}
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