// Copyright Epic Games, Inc. All Rights Reserved.


#include "LabyrinthGameModeBase.h"
#include "LabCharacter.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime\Engine\Classes\GameFramework\PlayerStart.h>
#include "..\..\Source\Labyrinth\SpawnRoom.h"
#include <Runtime\Engine\Public\EngineUtils.h>
#include <Labyrinth\LabGenerator.h>

ALabyrinthGameModeBase::ALabyrinthGameModeBase()
{
	// Change default class for the BP one
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
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
		if (Starts.Num() != 5) 
		{
			AActor* LabGen = UGameplayStatics::GetActorOfClass(GetWorld(), ALabGenerator::StaticClass());
			if (LabGen)
			{
				LabGen->DispatchBeginPlay();
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
			}

		}
		
	}

	if (Starts.Num() != 5 || !Player) // If we couldn't find all 4 playerStarts, delay spawn
	{
		WaitingPlayers.Add(Cast<APlayerController>(Player));
		return nullptr;
	}

	currentIndex++;
	return Starts[currentIndex-1];

}


