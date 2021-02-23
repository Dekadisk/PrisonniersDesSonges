// Copyright Epic Games, Inc. All Rights Reserved.


#include "LabyrinthGameModeBase.h"
#include "LabCharacter.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime\Engine\Classes\GameFramework\PlayerStart.h>

ALabyrinthGameModeBase::ALabyrinthGameModeBase()
{
	// Changer la classe de défaut pour celle définie par le blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(
		TEXT("/Game/Blueprints/LabCharacter_BP"));

	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

}

AActor* ALabyrinthGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{

	if (currentIndex == 0)
		UGameplayStatics::GetAllActorsOfClass(GetWorld(),APlayerStart::StaticClass(), Starts);
	if (Starts.Num() == 0 || !Player)
		return nullptr;
	currentIndex++;
	
	return Starts[currentIndex-1];

}
