#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "GameFramework/Character.h"
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

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement", Transient, Replicated)
	bool bNotSeenYet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mouvement", Transient, Replicated)
	bool bIsTrapped{ false };


	
	// Distance maximale de focus sur les objets.
	UPROPERTY(EditDefaultsOnly, Category = "ObjectInteraction")
	float MaxUseDistance;

	// Seulement vrai lors de la premi�re image avec un nouveau focus.
	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	bool bHasNewFocus;

	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	class AUsableActor* FocusedUsableActor;

	UPROPERTY(EditDefaultsOnly, Category = "FocusActor", Transient, Replicated)
	class ACachette* currentCachette;

	UPROPERTY(BlueprintReadWrite, Category = "FocusActor", Transient, Replicated)
	class AKeyPickUpActor* currentKey;

public:

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	UFUNCTION()
	void Use();

	UFUNCTION(NetMulticast, Reliable)
	void Trap();

	UFUNCTION(BlueprintCallable, NetMulticast, Reliable)
	void Untrap();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerUse();

	UFUNCTION()
	void AlternativeUse();

	UFUNCTION(Server, Reliable, WithValidation)
	void ServerAlternativeUse();

	UFUNCTION(NetMulticast, Reliable)
	void ClientUse(AUsableActor* Usable);

	UFUNCTION(NetMulticast, Reliable)
	void ClientAlternativeUse(AUsableActor* Usable);

	UFUNCTION(BlueprintCallable, Category = "Status")
	class AUsableActor* GetUsableInView();

	UFUNCTION(BlueprintCallable, Category = "Status")
	FHitResult GetPositionInView();

	//UFUNCTION()
	AActor* InstanceBP(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale = { 1.f,1.f,1.f });

	UFUNCTION()
	void Chat();

	UFUNCTION()
	void Pause();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
