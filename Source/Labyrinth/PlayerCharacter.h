#pragma once

#include "CoreMinimal.h"
#include "LabCharacter.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LABYRINTH_API APlayerCharacter : public ALabCharacter
{
	GENERATED_BODY()

public:
	

	/** Touche Run Active */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Stamina")
	bool bRunning;

	UPROPERTY(VisibleAnywhere, Category = "Stamina"/*, Transient, Replicated*/)
	bool bWaitFullRecovery;

	UPROPERTY(VisibleAnywhere, Category = "Stamina"/*, Transient, Replicated*/)
	unsigned int stamina;

	UPROPERTY(VisibleAnywhere, Category = "Stamina")
	unsigned int staminaMax;

private:

	const float BaseSpeed = 0.5f;
	const float RunSpeed = 1.0f;

	FTimerHandle timerHandle;

	FVector MovementDirection;

public:

	APlayerCharacter();

public:
	
	void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Forward(float Value);

	UFUNCTION()
	void Right(float Value);

	//Est ce qu'on cours ?
	UFUNCTION(BlueprintCallable, Category = "Mouvement")
	bool IsRunning();

	//On active la course
	UFUNCTION()
	void OnStartRun();

	// On d�sactive la course
	UFUNCTION()
	void OnStopRun();

	UFUNCTION()
	void LaunchStaminaRegen();

	UFUNCTION()
	void LaunchStaminaConsume();

	UFUNCTION(exec)
	void UsainBolt();

	UFUNCTION(exec)
	void GiveKey();

	/* Will check if player has a chalk ; if true, will allow them to select a spray and will draw it.
	Calls SelectionWheel functions.*/
	UFUNCTION()
	void ShowSelectionWheel();

	UFUNCTION()
	void UnShowSelectionWheel();

	UFUNCTION()
	void Draw();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpray(TypeDraw sprayType, FVector pos, FRotator sprayRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerClear(AActor* acteur);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnhide();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerHide();

	UFUNCTION(BlueprintImplementableEvent)
	void UnregisterCharacter();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DieFrom(AActor* Source = nullptr);

private:

	UFUNCTION()
	void RegenStamina();

	UFUNCTION()
	void ConsumeStamina();
};
