// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScriptActor.h"
#include "LabyrinthGameInstance.h"

void AMainMenuLevelScriptActor::BeginPlay() {

	ULabyrinthGameInstance* gameInstance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	gameInstance->ShowMainMenu();
}