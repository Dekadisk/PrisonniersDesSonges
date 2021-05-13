#include "LabyrinthGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "AIDirector.h"
#include "PlayerCharacter.h"
#include "LabyrinthPlayerController.h"
#include "EngineUtils.h"

ALabyrinthGameModeBase::ALabyrinthGameModeBase()
{
	bUseSeamlessTravel = true;
	// Change default class for the BP one
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(
		TEXT("/Game/Blueprints/PlayerCharacter_BP"));

	if (PlayerPawnObject.Class != NULL)
		DefaultPawnClass = PlayerPawnObject.Class;

	PlayerControllerClass = ALabyrinthPlayerController::StaticClass();	
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
	/*if (AIdirector->startInfluence == 0.0f)
		AIdirector->startInfluence = Cast<APlayerCharacter>(Cast<APlayerController>(player)->GetPawn())->InfluenceDataAsset->influence;*/

	Super::GenericPlayerInitialization(player);
}

bool ALabyrinthGameModeBase::EndGame() {

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut Endgame");
	bool everyoneDead = true;
	for (APlayerController* pc : AllPlayerControllers) {

		ALabyrinthPlayerController* labPC = Cast<ALabyrinthPlayerController>(pc);
		everyoneDead = everyoneDead && labPC->bIsDead;
	}

	if (everyoneDead) {
		GetWorld()->ServerTravel("/Game/ChildrensRoom/Maps/CutScene0");
	}

	return everyoneDead;
}

void ALabyrinthGameModeBase::HandleDeath() {
	/*ALabyrinthPlayerController* serverPC = nullptr;
	for (APlayerController* pc : AllPlayerControllers) {
		if (pc->GetNetMode() == ENetMode::NM_Client)
			Cast<ALabyrinthPlayerController>(pc)->Kicked();
		else
			serverPC = Cast<ALabyrinthPlayerController>(pc);
	}

	if (IsValid(serverPC)) {
		serverPC->EndPlay(EEndPlayReason::Quit);
		UGameplayStatics::OpenLevel(GetWorld(), FName("/Game/UI/Main"));
	}*/
}

void ALabyrinthGameModeBase::ActivateDebug()
{
	debug = !debug;
}

void ALabyrinthGameModeBase::Logout(AController* Exiting) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut logout");
	Super::Logout(Exiting);
	
	auto i = AllPlayerControllers.IndexOfByPredicate([&](APlayerController* pc) {
		return pc == Cast<APlayerController>(Exiting);
	});

	AllPlayerControllers.Remove(Cast<APlayerController>(Exiting));

	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "JME CASSE");
}

void ALabyrinthGameModeBase::AddPCs(AController* OldPC, AController* NewPC) {
	AllPlayerControllers.Add(Cast<APlayerController>(NewPC));
}

void ALabyrinthGameModeBase::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const {
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ALabyrinthGameModeBase, AllPlayerControllers);
}