// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthPlayerController.h"
#include "SelectionWheelUserWidget.h"

ALabyrinthPlayerController::ALabyrinthPlayerController()
{
	bHasKey = false;
	bHasLantern = false;
	bHasTrap = false;
	bHasChalk = false;

	static ConstructorHelpers::FClassFinder<UUserWidget> SelectionWheelWidget{ TEXT("/Game/UI/SelectionWheel") };
	SelectionWheelWidgetClass = SelectionWheelWidget.Class;

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial0(TEXT("/Game/Assets/SelectionWheel/SW.SW"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial1(TEXT("/Game/Assets/SelectionWheel/SW_C.SW_C"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial2(TEXT("/Game/Assets/SelectionWheel/SW_Cr.SW_Cr"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial3(TEXT("/Game/Assets/SelectionWheel/SW_I.SW_I"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial4(TEXT("/Game/Assets/SelectionWheel/SW_L.SW_L"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial5(TEXT("/Game/Assets/SelectionWheel/SW_R.SW_R"));
	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterial6(TEXT("/Game/Assets/SelectionWheel/SW_F.SW_F"));

	if (FoundMaterial0.Succeeded()) sprayArray.Add(FoundMaterial0.Object);
	if (FoundMaterial1.Succeeded()) sprayArray.Add(FoundMaterial1.Object);
	if (FoundMaterial2.Succeeded()) sprayArray.Add(FoundMaterial2.Object);
	if (FoundMaterial3.Succeeded()) sprayArray.Add(FoundMaterial3.Object);
	if (FoundMaterial4.Succeeded()) sprayArray.Add(FoundMaterial4.Object);
	if (FoundMaterial5.Succeeded()) sprayArray.Add(FoundMaterial5.Object);
	if (FoundMaterial6.Succeeded()) sprayArray.Add(FoundMaterial6.Object);

	
}

void ALabyrinthPlayerController::BeginPlay()
{
	if (IsLocalController())
	{
		SelectionWheel = CreateWidget<UUserWidget>(this, SelectionWheelWidgetClass);
	}
	SetInputMode(FInputModeGameOnly());
}

void ALabyrinthPlayerController::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALabyrinthPlayerController, bHasKey);
	DOREPLIFETIME(ALabyrinthPlayerController, bHasLantern);
	DOREPLIFETIME(ALabyrinthPlayerController, bHasTrap);
	DOREPLIFETIME(ALabyrinthPlayerController, bHasChalk);
}