#pragma once

#include "CoreMinimal.h"
#include "LabCharacter.h"
#include "ChalkDrawDecalActor.h"
#include "Perception/AISightTargetInterface.h"
#include "InfluenceDataAsset.h"
#include "Camera/CameraComponent.h"
#include "TrapActor.h"
#include "ChalkDrawDecalActor.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class LABYRINTH_API APlayerCharacter : public ALabCharacter, public IAISightTargetInterface
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

	UPROPERTY(EditAnywhere, Category = "InfluenceMap")
	UInfluenceDataAsset* InfluenceDataAsset;

	UPROPERTY(EditAnywhere, Category = "InfluenceMap")
	bool useThreat = true;

	UPROPERTY()
	float Threat;

	UPROPERTY()
	UCameraComponent* cameraSpecComp;

	UPROPERTY()
	UCameraComponent* cameraComp;

	UPROPERTY()
	AActor* trapHeld;

	UFUNCTION()
	virtual bool shouldUseAlternativeInfluence(); // overridable

	UPROPERTY(EditDefaultsOnly, Category = "LookAtActor", Transient, Replicated)
	class ALookAtTrigger* FocusedLookAtTrigger;

	UPROPERTY()
	TSubclassOf<ATrapActor> Trap_BP;
	UPROPERTY()
	TSubclassOf<AChalkDrawDecalActor> Spray_BP;

private:

	const float BaseSpeed = 0.5f;
	const float RunSpeed = 1.0f;

	FTimerHandle timerHandle;

	FVector MovementDirection;

public:

	APlayerCharacter();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

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

	// On active la course
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

	UFUNCTION(exec)
	void GiveTrap();

	UFUNCTION(exec)
	void GiveChalk();

	UFUNCTION(exec)
	void IAmBatman(int val);

	/* Will check if player has a chalk ; if true, will allow them to select a spray and will draw it.
	Calls SelectionWheel functions.*/
	UFUNCTION()
	void ShowSelectionWheel();

	UFUNCTION()
	void UnShowSelectionWheel();

	UFUNCTION()
	void ShowScoreboard();

	UFUNCTION()
	void UnShowScoreboard();

	UFUNCTION()
	void Draw();

	UFUNCTION()
	void SetTrap();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpray(TypeDraw sprayType, FVector pos, FRotator sprayRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSetTrap(FVector pos, FRotator sprayRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerClear(AActor* acteur);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUnhide();

	UFUNCTION(BlueprintCallable, Server, Reliable, WithValidation)
	void ServerHide();

	UFUNCTION(Reliable, NetMulticast)
	void MulticastPlayAnim(UAnimMontage* anim, int droite);

	UFUNCTION(BlueprintImplementableEvent)
	void UnregisterCharacter();

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DieFrom(AActor* Source = nullptr);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DieCachette();

	UFUNCTION(BlueprintCallable)
	void HandleDeath();

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;

	UFUNCTION(BlueprintCallable, Category = "Status")
	class ALookAtTrigger* GetLookAtInView();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
private:

	UFUNCTION()
	void RegenStamina();

	UFUNCTION()
	void ConsumeStamina();
};
