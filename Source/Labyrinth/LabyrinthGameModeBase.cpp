// Copyright Epic Games, Inc. All Rights Reserved.


#include "LabyrinthGameModeBase.h"
#include "LabCharacter.h"

ALabyrinthGameModeBase::ALabyrinthGameModeBase()
{
	// Changer la classe de d�faut pour celle d�finie par le blueprint
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(
		TEXT("/Game/Blueprints/LabCharacter_BP"));

	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

}