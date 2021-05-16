// Fill out your copyright notice in the Description page of Project Settings.


#include "BackEndScoreBoardWidget.h"
#include "LabyrinthGameInstance.h"

void UBackEndScoreBoardWidget::OnClickBack()
{
	RemoveFromParent();

	ULabyrinthGameInstance* instance = Cast<ULabyrinthGameInstance>(GetGameInstance());
	instance->ShowMainMenu();
}
