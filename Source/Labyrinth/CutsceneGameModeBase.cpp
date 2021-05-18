// Fill out your copyright notice in the Description page of Project Settings.

#include "CutsceneGameModeBase.h"
#include "GameFramework/SpectatorPawn.h"
#include "LabyrinthPlayerController.h"
#include "LabyrinthGameInstance.h"
#include "Kismet/GameplayStatics.h"

ACutsceneGameModeBase::ACutsceneGameModeBase()
{
	DefaultPawnClass = ASpectatorPawn::StaticClass();

	PlayerControllerClass = ALabyrinthPlayerController::StaticClass();
}

void ACutsceneGameModeBase::EndGame() {
	ULabyrinthGameInstance* GameInst = Cast<ULabyrinthGameInstance>(GetWorld()->GetGameInstance());

	UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
	GameInst->FromLobby = true;
	GameInst->DestroySession(GameInst->SessionName);	
}