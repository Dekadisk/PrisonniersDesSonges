#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/PlayerController.h"
#include "LabyrinthGameModeBase.generated.h"

UCLASS()
class LABYRINTH_API ALabyrinthGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:

	bool debug = false;

	ALabyrinthGameModeBase();

	UPROPERTY()
	TArray<AActor*> Starts;

	UPROPERTY()
	int currentIndex{};

	TArray<APlayerController*> WaitingPlayers;

	bool labGeneratorDone{false};
	
	UPROPERTY(Replicated)
	TArray<APlayerController*> AllPlayerControllers;

public:

	AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	void GenericPlayerInitialization(AController* player) override;

	void Logout(AController* Exiting) override;

	UFUNCTION(BlueprintCallable, Category = "LabGM")
	void AddPCs(AController* OldPC, AController* NewPC);

	UFUNCTION(BlueprintCallable, Category = "LabGM")
	bool EndGame();

	UFUNCTION(Exec)
	void ActivateDebug();

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:

	class AAIDirector* AIdirector = nullptr;

	FTimerHandle timerHandle;
	
	void HandleDeath();
};
