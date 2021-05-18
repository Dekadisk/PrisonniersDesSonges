#pragma once

#include "Core.h"
#include "PlayerCharacter.h"
#include "AkAudioEvent.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class LABYRINTH_API AMonsterCharacter : public ALabCharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMonsterCharacter();

	UPROPERTY(BlueprintReadWrite)
	AActor* Prey = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TauntAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DestroyCachetteAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* DestroyTrapAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TrappedAnim;

	UPROPERTY(EditDefaultsOnly)
	UAkAudioEvent* Respiration;

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool Chasing = false;

	UPROPERTY(BlueprintReadWrite, Replicated)
	bool Wandering = false;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackPlayer(APlayerCharacter* Target);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartHunt(APlayerCharacter* Target);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackCachette(class ACachette* Target);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastDestroyTrap(class ATrapActor* Target);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastTrapped(int droite, class ATrapActor* Target);

	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;

private:

	TSubclassOf<APawn> LabClassBP;

};
