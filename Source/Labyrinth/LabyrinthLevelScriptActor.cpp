// Fill out your copyright notice in the Description page of Project Settings.


#include "LabyrinthLevelScriptActor.h"
#include <Runtime/Engine/Classes/Kismet/GameplayStatics.h>

ALabyrinthLevelScriptActor::ALabyrinthLevelScriptActor() {
	static ConstructorHelpers::FClassFinder<UUserWidget> MenuWidget{ TEXT("/Game/UI/MainMenu") };
	MenuWidgetClass = MenuWidget.Class;
}

void ALabyrinthLevelScriptActor::BeginPlay() {

	UUserWidget* MainMenu = CreateWidget<UUserWidget>(GetWorld(), MenuWidgetClass);
	MainMenu->AddToViewport();

	UGameplayStatics::GetPlayerController(GetWorld(), 0)->SetShowMouseCursor(true);
}