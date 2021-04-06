#include "LabyrinthGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework\PlayerStart.h"
#include "LabGenerator.h"
#include "LabyrinthGameStateBase.h"
#include "AIDirector.h"
#include "LabyrinthPlayerController.h"
#include "EngineUtils.h"

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

	if (currentIndex == 0)
	{
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
		int a = 1;
		a++;
		if (Starts.Num() != 4) 
		{
			AActor* LabGen = UGameplayStatics::GetActorOfClass(GetWorld(), ALabGenerator::StaticClass());
			if (LabGen)
			{
				LabGen->DispatchBeginPlay();
				UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
			}

		}
		
	}

	if (Starts.Num() != 4 || !Player) // If we couldn't find all 4 playerStarts, delay spawn
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
