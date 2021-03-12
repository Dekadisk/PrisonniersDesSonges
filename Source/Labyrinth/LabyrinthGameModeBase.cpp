// Copyright Epic Games, Inc. All Rights Reserved.


#include "LabyrinthGameModeBase.h"
#include "LabCharacter.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <Runtime\Engine\Classes\GameFramework\PlayerStart.h>
#include "..\..\Source\Labyrinth\SpawnRoom.h"
#include <Runtime\Engine\Public\EngineUtils.h>
#include <Labyrinth\LabGenerator.h>
#include "LabyrinthGameStateBase.h"
#include "AIDirector.h"
#include "PlayerCharacter.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <vector>
#include <EngineUtils.h>
ALabyrinthGameModeBase::ALabyrinthGameModeBase()
{
	// Change default class for the BP one
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(
		TEXT("/Game/Blueprints/RunnerCharacter_BP"));

	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	GameStateClass = ALabyrinthGameStateBase::StaticClass();
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





void ALabyrinthGameModeBase::PostLogin(APlayerController* player) {

	UWorld* World = GetWorld();
	bool hasDirector = false;
	if (AIdirector == nullptr)
	{
		AActor* director = nullptr;
		for (FActorIterator It(World); It; ++It)
		{
			if (Cast<AAIDirector>(*It)) {
				hasDirector = true;
				director = *It;
				director->DispatchBeginPlay(true);
				AIdirector = Cast<AAIDirector>(director);
			}
		}
	}

	if(hasDirector)
		AIdirector->AddPlayer(player);

	Super::PostLogin(player);
}

void ALabyrinthGameModeBase::ActivateDebug()
{
	debug = !debug;
}
