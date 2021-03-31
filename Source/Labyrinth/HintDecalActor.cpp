#include "HintDecalActor.h"
#include "PuzzleActor.h"
#include "SolvableActor.h"
#include "Components/DecalComponent.h"

AHintDecalActor::AHintDecalActor()
{
	bReplicates = true;

	decalClockOrder = CreateDefaultSubobject<UDecalComponent>(TEXT("ClockOrder"));
	decalClockOrder->SetupAttachment(GetRootComponent());

	// ASSETS
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial0(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_0.M_Hint_0"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial1(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_1.M_Hint_1"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial2(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_2.M_Hint_2"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial3(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_3.M_Hint_3"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial4(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_4.M_Hint_4"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial5(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_5.M_Hint_5"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial6(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_6.M_Hint_6"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial7(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_Hint_7.M_Hint_7"));

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial8(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_I.M_I"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial9(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_II.M_II"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial10(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_III.M_III"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial11(TEXT("/Game/Assets/Usable/Puzzle/Clock/M_IV.M_IV"));

	if (FoundMaterial0.Succeeded()) matHints.Add(FoundMaterial0.Object);
	if (FoundMaterial1.Succeeded()) matHints.Add(FoundMaterial1.Object);
	if (FoundMaterial2.Succeeded()) matHints.Add(FoundMaterial2.Object);
	if (FoundMaterial3.Succeeded()) matHints.Add(FoundMaterial3.Object);
	if (FoundMaterial4.Succeeded()) matHints.Add(FoundMaterial4.Object);
	if (FoundMaterial5.Succeeded()) matHints.Add(FoundMaterial5.Object);
	if (FoundMaterial6.Succeeded()) matHints.Add(FoundMaterial6.Object);
	if (FoundMaterial7.Succeeded()) matHints.Add(FoundMaterial7.Object);

	if (FoundMaterial8.Succeeded()) matHintsClockNb.Add(FoundMaterial8.Object);
	if (FoundMaterial9.Succeeded()) matHintsClockNb.Add(FoundMaterial9.Object);
	if (FoundMaterial10.Succeeded()) matHintsClockNb.Add(FoundMaterial10.Object);
	if (FoundMaterial11.Succeeded()) matHintsClockNb.Add(FoundMaterial11.Object);
}

void AHintDecalActor::OnRep_UpdateMaterial()
{
	switch (kind) {
	case TypeHint::CLOCK_POS0:
		SetDecalMaterial(matHints[0]);
		break;
	case TypeHint::CLOCK_POS1:
		SetDecalMaterial(matHints[1]);
		break;
	case TypeHint::CLOCK_POS2:
		SetDecalMaterial(matHints[2]);
		break;
	case TypeHint::CLOCK_POS3:
		SetDecalMaterial(matHints[3]);
		break;
	case TypeHint::CLOCK_POS4:
		SetDecalMaterial(matHints[4]);
		break;
	case TypeHint::CLOCK_POS5:
		SetDecalMaterial(matHints[5]);
		break;
	case TypeHint::CLOCK_POS6:
		SetDecalMaterial(matHints[6]);
		break;
	case TypeHint::CLOCK_POS7:
		SetDecalMaterial(matHints[7]);
		break;
	case TypeHint::ECLOCK_POS0:
		SetDecalMaterial(matHints[0 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS1:
		SetDecalMaterial(matHints[1 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS2:
		SetDecalMaterial(matHints[2 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS3:
		SetDecalMaterial(matHints[3 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS4:
		SetDecalMaterial(matHints[4 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS5:
		SetDecalMaterial(matHints[5 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS6:
		SetDecalMaterial(matHints[6 + decalErase]);
		break;
	case TypeHint::ECLOCK_POS7:
		SetDecalMaterial(matHints[7 + decalErase]);
		break;
	default:
		Destroy();
		break;
	}
}

void AHintDecalActor::OnRep_UpdateMaterialOrder()
{
	switch (clockOrder)
	{
	case ClockOrder::CLOCK_I:
		decalClockOrder->SetDecalMaterial(matHintsClockNb[0]);
		break;
	case ClockOrder::CLOCK_II:
		decalClockOrder->SetDecalMaterial(matHintsClockNb[1]);
		break;
	case ClockOrder::CLOCK_III:
		decalClockOrder->SetDecalMaterial(matHintsClockNb[2]);
		break;
	case ClockOrder::CLOCK_IV:
		decalClockOrder->SetDecalMaterial(matHintsClockNb[3]);
		break;
	case ClockOrder::UNDEFINED:
		break;
	default:
		break;
	}
}

void AHintDecalActor::Erase()
{
	isErased = true;
	kind = static_cast<TypeHint>(static_cast<unsigned int>(kind) + decalErase);
	puzzleActor->targetActor[0].linkedActor->hasErasedHint = true;
}

void AHintDecalActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AHintDecalActor, kind);
	DOREPLIFETIME(AHintDecalActor, clockOrder);
}

void AHintDecalActor::BeginPlay()
{
	Super::BeginPlay();
	decalClockOrder->SetRelativeScale3D({ 1.f, 0.2f, 0.2f });
	decalClockOrder->SetRelativeLocation({ 0, -1.1f, 0 });
}
