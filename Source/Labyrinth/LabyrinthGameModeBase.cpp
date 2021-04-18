#include "LabyrinthGameModeBase.h"
#include "Kismet\GameplayStatics.h"
#include "GameFramework\PlayerStart.h"
#include "LabGenerator.h"
#include "LabyrinthGameStateBase.h"
#include "AIDirector.h"
#include "LabyrinthPlayerController.h"
#include "EngineUtils.h"
#include "LabyrinthGameInstance.h"

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
	if(!currentIndex)
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
	return Starts[currentIndex++];

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

void ALabyrinthGameModeBase::Logout(AController* Exiting) {
	Super::Logout(Exiting);

	auto i = AllPlayerControllers.IndexOfByPredicate([&](APlayerController* pc) {
		return pc == Cast<APlayerController>(Exiting);
		});

	AllPlayerControllers.Remove(Cast<APlayerController>(Exiting));

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "JME CASSE");
}

void ALabyrinthGameModeBase::AddPCs(AController* OldPC, AController* NewPC) {
	AllPlayerControllers.Add(Cast<APlayerController>(NewPC));
}

void ALabyrinthGameModeBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALabyrinthGameModeBase, AllPlayerControllers);
}