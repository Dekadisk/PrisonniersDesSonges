#pragma once

#include "Core.h"
#include "PlayerCharacter.h"
#include "MonsterCharacter.generated.h"

UCLASS()
class LABYRINTH_API AMonsterCharacter : public ALabCharacter
{
	GENERATED_BODY()

public:

	// Sets default values for this character's properties
	AMonsterCharacter();

	UPROPERTY(BlueprintReadWrite)
	APlayerCharacter* Prey = nullptr;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* AttackAnim;

	UPROPERTY(EditDefaultsOnly)
	UAnimMontage* TauntAnim;

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackPlayer(APlayerCharacter* Target);

	UFUNCTION(NetMulticast, Reliable)
	void MulticastStartHunt(APlayerCharacter* Target);

private:

	TSubclassOf<APawn> LabClassBP;

};
