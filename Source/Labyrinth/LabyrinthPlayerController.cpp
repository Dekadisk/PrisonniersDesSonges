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

	static ConstructorHelpers::FObjectFinder<UMaterial> FoundMaterialSW(TEXT("/Game/Assets/SelectionWheel/SW.SW"));

	if (FoundMaterialSW.Succeeded()) SelectionWheelMaterial = FoundMaterialSW.Object;
}

void ALabyrinthPlayerController::BeginPlay()
{
	SetInputMode(FInputModeGameOnly());
	if(IsLocalController())
		SelectionWheel = CreateWidget<UUserWidget>(this, SelectionWheelWidgetClass);

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