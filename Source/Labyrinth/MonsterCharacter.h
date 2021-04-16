#pragma once

#include "Core.h"
#include "Net/UnrealNetwork.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
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

	UFUNCTION(NetMulticast, Reliable)
	void MulticastAttackPlayer(APlayerCharacter* Target);

protected:

private:

	TSubclassOf<APawn> LabClassBP;

};
