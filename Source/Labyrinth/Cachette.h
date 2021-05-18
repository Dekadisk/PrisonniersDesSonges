#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "PlayerCharacter.h"
#include "Perception/AISightTargetInterface.h"
#include "Cachette.generated.h"

UCLASS()
class LABYRINTH_API ACachette : public AUsableActor, public IAISightTargetInterface
{
	GENERATED_BODY()

public:
	ACachette();

public:

	// Meshes
	//

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Hitboite;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PorteG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* PorteD;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* TiroirG;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* TiroirD;

	// Array of players in Cachette
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "InCupboardPlayers")
		TArray<APlayerCharacter*> InCupboardPlayers;

	// Useful booleans
	//
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsOpen;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Status")
	bool bIsProcessing;

	FTimerHandle timerHandle;
	unsigned int timeIABreak{};

	// Functions
	//
	virtual void Use(bool Event, APawn* InstigatorPawn = nullptr);

	UFUNCTION()
	void BeginOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp,
		class AActor* OtherActor,
		class UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex);

	// Custom Events
	//
	UFUNCTION(BlueprintImplementableEvent)
	void SlightlyOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void AllOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void Close();

	UFUNCTION()
	void OnBeginFocus();

	UFUNCTION()
	void OnEndFocus();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastOpen();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastClose();

	UFUNCTION(NetMulticast, Reliable)
	void MulticastRemovePlayer(APlayerCharacter* player);

	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable)
	void DestroyCachette();

	virtual void BeginPlay() override;
	virtual void Tick(float a) override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void LaunchIAOpen();

	UFUNCTION()
	void IAWait();

	virtual bool CanBeSeenFrom(const FVector& ObserverLocation, FVector& OutSeenLocation, int32& NumberOfLoSChecksPerformed, float& OutSightStrength, const AActor* IgnoreActor = NULL) const override;

};
