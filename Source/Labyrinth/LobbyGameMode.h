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
	
	UPROPERTY(Transient, Replicated)
	TArray<APlayerController*> AllPlayerControllers;

	UPROPERTY(BluePrintReadWrite)
	bool canStart = false;

	UPROPERTY(Transient, Replicated)
	TArray<AActor*> playerStarts;

	UPROPERTY(Replicated)
	FName ServerName;

	UPROPERTY(Replicated)
	int seed;

	UPROPERTY(Replicated)
	int nbPlayers;

	UPROPERTY(Replicated)
	TArray<FPlayerInfo> playersInfo;

	UPROPERTY(Replicated)
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

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void PostLogin(APlayerController* newPlayer) override;

	void Logout(AController* Exiting) override;
	
};
