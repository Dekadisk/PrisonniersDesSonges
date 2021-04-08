#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "AIDirector.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/PlayerStart.h"
#include "LabyrinthGameModeBase.generated.h"

UCLASS()
class LABYRINTH_API ALabyrinthGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	bool debug = false;

	ALabyrinthGameModeBase();

	TArray<AActor*> Starts;
	int currentIndex{};

	TArray<APlayerController*> WaitingPlayers;

	bool labGeneratorDone{false};
	
public:

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void PostLogin(APlayerController* player) override;

	UFUNCTION(Exec)
	void ActivateDebug();

	/*UFUNCTION()
	void SpawnPlayers();

	void Tick(float somefloat) override;*/

private:

	AAIDirector* AIdirector = nullptr;
	
};
