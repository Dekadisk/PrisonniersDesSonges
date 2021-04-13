#pragma once

#include "Core.h"
#include "GameFramework/GameMode.h"
#include "PlayerInfo.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class LABYRINTH_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	ALobbyGameMode();
	
	UPROPERTY()
	TArray<APlayerController*> AllPlayerControllers;

	UPROPERTY(BluePrintReadWrite)
	bool canStart = false;

	UPROPERTY()
	TArray<AActor*> playerStarts;

	UPROPERTY()
	FName ServerName;

	UPROPERTY()
	int seed;

	UPROPERTY()
	int nbPlayers;

	UPROPERTY()
	TArray<FPlayerInfo> playersInfo;

	UPROPERTY()
	FText ServerPlayerName;

	UFUNCTION()
	void LaunchGame();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEveryoneUpdate();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRespawnPlayer(APlayerController* pc);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUpdateGameSettings(int _seed);

	UFUNCTION(Server, Reliable)
	void ServerGetKicked(int id);

	void PostLogin(APlayerController* newPlayer) override;

	void Logout(AController* Exiting) override;
	
};
