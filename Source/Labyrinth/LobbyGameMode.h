#pragma once

#include "Core.h"
#include "GameFramework/GameMode.h"
#include "LobbyGameMode.generated.h"

UCLASS()
class LABYRINTH_API ALobbyGameMode : public AGameMode
{
	GENERATED_BODY()

public:
	UPROPERTY(Transient, Replicated)
	TArray<APlayerController*> AllPlayerControllers;

	UPROPERTY(BluePrintReadWrite)
	bool canWeStart = false;

	UPROPERTY(Transient, Replicated)
	TArray<AActor*> playerStarts;

	UPROPERTY(Replicated)
	FText ServerName;

	UPROPERTY(Replicated)
	int seed;

	UFUNCTION()
	void LaunchGame();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEveryoneUpdate();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerRespawnPlayer(APlayerController* pc);

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	void PostLogin(APlayerController* newPlayer) override;
	
};
