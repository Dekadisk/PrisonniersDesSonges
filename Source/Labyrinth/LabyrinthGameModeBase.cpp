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

	bUseSeamlessTravel = true;
}

AActor* ALabyrinthGameModeBase::ChoosePlayerStart_Implementation(AController* Player)
{
	if(!currentIndex)
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), Starts);
	return Starts[currentIndex++];

}

void ALabyrinthGameModeBase::GenericPlayerInitialization(AController* player)
{
	UWorld* World = GetWorld();

	if (AIdirector == nullptr)
	{
		AActor* director = nullptr;
		for (FActorIterator It(World); It; ++It)
		{
			if (Cast<AAIDirector>(*It)) {
				director = *It;
				director->DispatchBeginPlay(true);
				AIdirector = Cast<AAIDirector>(director);
			}
		}
	}

	if (AIdirector)
		AIdirector->AddPlayer(player);

	Super::GenericPlayerInitialization(player);
}

bool ALabyrinthGameModeBase::EndGame() {

	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut Endgame");
	bool everyoneDead = true;
	for (APlayerController* pc : AllPlayerControllers) {

		ALabyrinthPlayerController* labPC = Cast<ALabyrinthPlayerController>(pc);
		everyoneDead = everyoneDead && labPC->bIsDead;
	}

	if (everyoneDead) {
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut Travel");
		//GetWorld()->ServerTravel("/Game/Lobby");
		//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Fin Travel");

		for (APlayerController* pc : AllPlayerControllers)
			Cast<ALabyrinthPlayerController>(pc)->ShowDeathScreen();

		GetWorld()->GetTimerManager().ClearTimer(timerHandle);
		GetWorld()->GetTimerManager().SetTimer(timerHandle, this, &ALabyrinthGameModeBase::HandleDeath, 3.f, false);

	}

	return everyoneDead;
}

void ALabyrinthGameModeBase::HandleDeath() {
	ALabyrinthPlayerController* serverPC = nullptr;
	for (APlayerController* pc : AllPlayerControllers) {
		if (pc->GetNetMode() == ENetMode::NM_Client)
			Cast<ALabyrinthPlayerController>(pc)->Kicked();
		else
			serverPC = Cast<ALabyrinthPlayerController>(pc);
	}

	if (IsValid(serverPC)) {
		serverPC->EndPlay(EEndPlayReason::Quit);
		UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
	}
}

void ALabyrinthGameModeBase::ActivateDebug()
{
	debug = !debug;
}

void ALabyrinthGameModeBase::Logout(AController* Exiting) {
	GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut logout");
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