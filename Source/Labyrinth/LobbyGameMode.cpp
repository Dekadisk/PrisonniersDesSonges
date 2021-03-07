#include "LobbyGameMode.h"
#include <Net/UnrealNetwork.h>
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include <Runtime/Engine/Classes/GameFramework/PlayerStart.h>
#include "LabyrinthGameInstance.h"
#include "LobbyPlayerController.h"
//#include "PlayerCharacter.h"

void ALobbyGameMode::PostLogin(APlayerController* newPlayer) {
	if (HasAuthority()) {
		AllPlayerControllers.Add(newPlayer);

		UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), playerStarts);
		ServerName = Cast<ULabyrinthGameInstance>(GetGameInstance())->GetServerName();

		ALobbyPlayerController* lobbyPC = Cast<ALobbyPlayerController>(newPlayer);
		lobbyPC->InitialSetup();
		lobbyPC->SetupLobbyMenu(ServerName);
		lobbyPC->UpdateLocalSettings(seed);
		
		ServerRespawnPlayer(newPlayer);
	}
}

void ALobbyGameMode::ServerRespawnPlayer_Implementation(APlayerController* pc) {
	pc->GetPawn()->Destroy();

	//GetWorld()->SpawnActor(APlayerCharacter, )
}

bool ALobbyGameMode::ServerRespawnPlayer_Validate(APlayerController*) {
	return true;
}

void ALobbyGameMode::ServerEveryoneUpdate_Implementation() {

}

bool ALobbyGameMode::ServerEveryoneUpdate_Validate() {
	return true;
}

void ALobbyGameMode::LaunchGame()
{
}

void ALobbyGameMode::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALobbyGameMode, AllPlayerControllers);
}
