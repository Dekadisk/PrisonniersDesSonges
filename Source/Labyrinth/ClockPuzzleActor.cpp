#include "ClockPuzzleActor.h"

AClockPuzzleActor::AClockPuzzleActor() {
	ClockBase = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockBase"));
	ClockCenter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ClockCenter"));
	clockNumberDecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("ClockOrder"));

	ClockBase->AttachTo(MeshComp);
	ClockCenter->AttachTo(MeshComp);
	clockNumberDecalComponent->AttachTo(MeshComp);

	maxPos = 7;
	clockNumber = -1;



	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial8(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_I.M_I"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial9(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_II.M_II"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial10(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_III.M_III"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial11(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_IV.M_IV"));

	if (FoundMaterial8.Succeeded()) matHintsClockNb.Add(FoundMaterial8.Object);
	if (FoundMaterial9.Succeeded()) matHintsClockNb.Add(FoundMaterial9.Object);
	if (FoundMaterial10.Succeeded()) matHintsClockNb.Add(FoundMaterial10.Object);
	if (FoundMaterial11.Succeeded()) matHintsClockNb.Add(FoundMaterial11.Object);

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
		MulticastUpdateCurrentPos();
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

void AClockPuzzleActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AClockPuzzleActor, startPos);
	DOREPLIFETIME(AClockPuzzleActor, clockNumber);
}

void AClockPuzzleActor::OnRep_UpdateStartPos()
{
	FRotator NewRotator = FRotator::ZeroRotator; // starts with everything as 0.0f
	NewRotator.Roll = startPos * -45.0f; // new value of 10.0f
	ClockCenter->SetRelativeRotation(NewRotator);
	currPos = startPos;
}

void AClockPuzzleActor::OnRep_UpdateCurrentPos()
{
	Rotate();
}

void AClockPuzzleActor::OnRep_UpdateDecalMaterial()
{
	clockNumberDecalComponent->SetDecalMaterial(matHintsClockNb[clockNumber]);
}

void AClockPuzzleActor::MulticastUpdateCurrentPos_Implementation()
{
	Rotate();
	currPos = (currPos + 1) % (maxPos + 1);
}