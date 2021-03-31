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
	TArray<APlayerController*> AllPlayerController;

	UPROPERTY(BluePrintReadWrite)
	bool canWeStart = false;

	UFUNCTION()
	void LaunchGame();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerEveryoneUpdate();

	void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
};
