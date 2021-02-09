// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthLevelScriptActor.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuUserWidget.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

void ALabyrinthLevelScriptActor::BeginPlay() {

	UMainMenuUserWidget* MainMenu = CreateWidget<UMainMenuUserWidget>(GetWorld(), UMainMenuUserWidget::StaticClass());
	MainMenu->AddToViewport();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}