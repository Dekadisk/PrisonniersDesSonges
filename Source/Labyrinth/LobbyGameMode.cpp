#include "LobbyGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerStart.h"
#include "LabyrinthGameInstance.h"
#include "LobbyPlayerController.h"
#include "PlayerCharacter.h"
#include "Kismet/KismetMathLibrary.h"

ALobbyGameMode::ALobbyGameMode() {
	bUseSeamlessTravel = true;

	nbPlayers = 0;

	DefaultPawnClass = ACharacter::StaticClass();

	PlayerControllerClass = ALobbyPlayerController::StaticClass();
}

void ALobbyGameMode::PostLogin(APlayerController* newPlayer) {
	//GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "Debut LobbyGM");
	if (HasAuthority()) {
		AllPlayerControllers.Add(newPlayer);

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
		ServerName = Cast<ULabyrinthGameInstance>(GetGameInstance())->GetServerName();

		ALobbyPlayerController* lobbyPC = Cast<ALobbyPlayerController>(newPlayer);

		lobbyPC->InitialSetup();
		lobbyPC->SetupLobbyMenu(ServerName);
		lobbyPC->UpdateLocalSettings(seed);
		
		ServerRespawnPlayer(newPlayer);

		ServerPlayerName = playersInfo[0].PlayerName;
	}
}

void ALobbyGameMode::ServerRespawnPlayer_Implementation(APlayerController* pc) {

	if (pc->GetPawn()->IsValidLowLevel())
		pc->GetPawn()->Destroy();	

	int startIndex = UKismetMathLibrary::RandomIntegerInRange(0, playerStarts.Num() - 1);
	AActor* start = playerStarts[startIndex];
	APlayerCharacter* spawned = GetWorld()->SpawnActor<APlayerCharacter>(APlayerCharacter::StaticClass(), start->GetActorTransform());

	pc->Possess(spawned);
	ServerEveryoneUpdate();
}

bool ALobbyGameMode::ServerRespawnPlayer_Validate(APlayerController*) {
	return true;
}

void ALobbyGameMode::ServerEveryoneUpdate_Implementation() {
	nbPlayers = AllPlayerControllers.Num();

	if (nbPlayers > 0) {
		canStart = true;
		playersInfo.Empty();

		for (APlayerController* pc : AllPlayerControllers) {
			ALobbyPlayerController* lobbyPC = Cast<ALobbyPlayerController>(pc);
			playersInfo.Add(lobbyPC->playerSettings);
			lobbyPC->UpdateNumberPlayerDisplay(nbPlayers);
		}

		for (APlayerController* pc : AllPlayerControllers) {
			ALobbyPlayerController* lobbyPC = Cast<ALobbyPlayerController>(pc);
			lobbyPC->AddPlayerInfo(playersInfo);
		}

		for (FPlayerInfo pi : playersInfo)
			canStart = canStart && pi.PlayerStatus;
	}
}

bool ALobbyGameMode::ServerEveryoneUpdate_Validate() {
	return true;
}

void ALobbyGameMode::ServerUpdateGameSettings_Implementation(int _seed) {
	seed = _seed;

	for (APlayerController* pc : AllPlayerControllers) {
		ALobbyPlayerController* lobbyPC = Cast<ALobbyPlayerController>(pc);
		lobbyPC->UpdateLocalSettings(seed);
	}
}

bool ALobbyGameMode::ServerUpdateGameSettings_Validate(int) {
	return true;
}

void ALobbyGameMode::LaunchGame()
{
	if (!seed)
		seed = FMath::RandRange(INT32_MIN, INT32_MAX);

	for (APlayerController* pc : AllPlayerControllers) {
		ALobbyPlayerController* lobbyPC = Cast<ALobbyPlayerController>(pc);
		lobbyPC->SaveSeed(seed);
	}

	GetWorld()->ServerTravel("/Game/procedural_level");
}

void ALobbyGameMode::ServerGetKicked_Implementation(int id) {
	Cast<ALobbyPlayerController>(AllPlayerControllers[id])->Kicked();
}

void ALobbyGameMode::Logout(AController* Exiting) {
	
	//GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Red, "C'EST LE LOBBY LA");
	Super::Logout(Exiting);

	auto i = AllPlayerControllers.IndexOfByPredicate([&](APlayerController* pc) {
		return pc == Cast<APlayerController>(Exiting);
	});

	AllPlayerControllers.Remove(Cast<APlayerController>(Exiting));

	playersInfo.RemoveAt(i);

	ServerEveryoneUpdate();
}
