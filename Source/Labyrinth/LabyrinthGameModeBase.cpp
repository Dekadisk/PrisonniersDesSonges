#include "LabyrinthGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework\PlayerStart.h"
#include "LabGenerator.h"
#include "LabyrinthGameStateBase.h"
#include "AIDirector.h"
#include "LabyrinthPlayerController.h"
#include "PlayerCharacter.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include <vector>
#include "Engine/World.h"
#include <EngineUtils.h>

ALabyrinthGameModeBase::ALabyrinthGameModeBase()
{
	// Change default class for the BP one
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(
		TEXT("/Game/Blueprints/PlayerCharacter_BP"));

	if (PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	PlayerControllerClass = ALabyrinthPlayerController::StaticClass();

	GameStateClass = ALabyrinthGameStateBase::StaticClass();
}

AActor* ALabyrinthGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	return nullptr;
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
	//SpawnPlayers();
}

void ALabyrinthGameModeBase::ActivateDebug()
{
	debug = !debug;
}

//void ALabyrinthGameModeBase::SpawnPlayers()
//{
//	int pawnId = 0;
//	FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator();
//	for (; Iterator; ++Iterator)
//	{
//		APlayerController* PlayerActor = Iterator->Get();
//		if (PlayerActor && PlayerActor->PlayerState && !MustSpectate(PlayerActor))
//		{
//			PlayerActor->GetPawn()->SetActorLocation(Starts[pawnId++]->GetActorLocation());
//		}
//	}
//}
//
//void ALabyrinthGameModeBase::Tick(float somefloat)
//{
//	Super::Tick(somefloat);
//
//}
