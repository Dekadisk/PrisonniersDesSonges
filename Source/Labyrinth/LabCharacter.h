#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
#include "ChalkDrawDecalActor.h"
#include "LabCharacter.generated.h"

UCLASS()
class LABYRINTH_API ALabCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	ALabCharacter();

public:

	/** Vitesse de d�placement */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement")
	float Vitesse;

	/** Touche Run Active */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement")
	bool bPressedRun;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement", Transient, Replicated)
	bool bNotSeenYet;

	// Distance maximale de focus sur les objets.
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	// Seulement vrai lors de la premi�re image avec un nouveau focus.
	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	bool bHasNewFocus;

	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	class AUsableActor* FocusedUsableActor;

	class AHideSpotActor* currentHideSpot;

protected:

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Forward(float Value);

	UFUNCTION()
	void Right(float Value);

	//On active le bool�en bPressedJump
	UFUNCTION()
	void OnStartJump();
	// On d�sactive le bool�en bPressedJump
	UFUNCTION()
	void OnStopJump();

	//On active le bool�en bPressedRun
	UFUNCTION(BlueprintCallable, Category = "Mouvement")
	bool IsRunning();

	//On active la course
	UFUNCTION()
	void OnStartRun();
	// On d�sactive la course
	UFUNCTION()
	void OnStopRun();

	UFUNCTION()
	void Use();

	//UFUNCTION()
	//	void Spray(float Angle);

	/* Will check if player has a chalk ; if true, will allow them to select a spray and will draw it.
	Calls SelectionWheel functions.*/
	UFUNCTION()
	void ShowSelectionWheel();

	UFUNCTION()
	void UnShowSelectionWheel();

	UFUNCTION()
	void Draw();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	void ServerUse_Implementation();
	bool ServerUse_Validate();

	UFUNCTION()
	void AlternativeUse();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAlternativeUse();

	void ServerAlternativeUse_Implementation();
	bool ServerAlternativeUse_Validate();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerSpray(TypeDraw sprayType, FVector pos, FRotator sprayRotation);

	void ServerSpray_Implementation(TypeDraw sprayType, FVector pos, FRotator sprayRotation);
	bool ServerSpray_Validate(TypeDraw sprayType, FVector pos, FRotator sprayRotation);

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerClear(AActor* acteur);

	void ServerClear_Implementation(AActor* acteur);
	bool ServerClear_Validate(AActor* acteur);

	UFUNCTION(NetMulticast, Reliable)
	void ClientUse(AUsableActor* Usable);

	void ClientUse_Implementation(AUsableActor* Usable);

	UFUNCTION(NetMulticast, Reliable)
	void ClientAlternativeUse(AUsableActor* Usable);

	void ClientAlternativeUse_Implementation(AUsableActor* Usable);

	UFUNCTION(BlueprintCallable, Category = "Status")
	class AUsableActor* GetUsableInView();

	UFUNCTION(BlueprintCallable, Category = "Status")
	FHitResult GetPositionInView();

	AActor* InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale = { 1.f,1.f,1.f });

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
