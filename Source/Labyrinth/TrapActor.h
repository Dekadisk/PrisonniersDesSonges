#pragma once

#include "CoreMinimal.h"
#include "PickUpActor.h"
#include "LabCharacter.h"
#include "TrapHeld.h"
#include "Components/BoxComponent.h"
#include "TrapActor.generated.h"

UCLASS()
class LABYRINTH_API ATrapActor : public APickUpActor
{
	GENERATED_BODY()
	
	ATrapActor();
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* JawLeft;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* JawRight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* JawBar;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* JawButton;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh")
		UStaticMeshComponent* Hitbox;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Collision")
		UBoxComponent* OverlapAI;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "TrappedActor", Transient, Replicated)
		ALabCharacter* trappedCharacter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Transient, Replicated)
	bool bIsOpen{ true };

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TrapAnim;

	FTimerHandle timerHandle;
	unsigned int timeIABreak{};

	//BLUEPRINTS
	UPROPERTY()
	TSubclassOf<ATrapHeld> TrapHeld_BP;

	UFUNCTION(BlueprintImplementableEvent)
	void Open();

	UFUNCTION(BlueprintImplementableEvent)
	void SlowlyOpen();

	UFUNCTION(BlueprintImplementableEvent)
	void Close();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastOpen();

	UFUNCTION(Server, Reliable)
		void Free();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastClose();

	UFUNCTION(NetMulticast, Reliable)
		void MulticastSlowlyOpen();

	void Use(bool Event, APawn* InstigatorPawn = nullptr) override;

	UFUNCTION()
	virtual void BeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION()
	virtual void OnOverlapEnd(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void OnBeginFocus();

	UFUNCTION()
	void OnEndFocus();

	UFUNCTION(BlueprintCallable, Category = "Destroy")
	void DestroyTrap();

	AActor* SpawnHeld_BP();

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

	void IAWait();
};
