// Fill out your copyright notice in the Description page of Project Settings.

#include "CutsceneGameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "LabyrinthPlayerController.h"
#include "Kismet/GameplayStatics.h"

ACutsceneGameModeBase::ACutsceneGameModeBase()
{
	DefaultPawnClass = ASpectatorPawn::StaticClass();

	PlayerControllerClass = ALabyrinthPlayerController::StaticClass();

	bUseSeamlessTravel = true;
}

void ACutsceneGameModeBase::GenericPlayerInitialization(AController* player)
{
	Super::GenericPlayerInitialization(player);
	AllPlayerControllers.Add(Cast<APlayerController>(player));
	DisableInput(Cast<APlayerController>(player));
}

void ACutsceneGameModeBase::EndGame() {
	ALabyrinthPlayerController* serverPC = nullptr;
	for (APlayerController* pc : AllPlayerControllers) {
		if (pc->GetNetMode() == ENetMode::NM_Client)
			Cast<ALabyrinthPlayerController>(pc)->Kicked();
		else
			serverPC = Cast<ALabyrinthPlayerController>(pc);
	}

	if (IsValid(serverPC)) {
		serverPC->EndPlay(EEndPlayReason::Quit);
		UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
	}
}