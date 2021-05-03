#include "MonsterCharacter.h"
#include "Cachette.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Vitesse = 0.1f;
	MaxUseDistance = 800;

	static ConstructorHelpers::FClassFinder<APawn> LabBP{ TEXT("/Game/Blueprints/PlayerCharacter_BP") };
	LabClassBP = LabBP.Class;
}


void AMonsterCharacter::MulticastAttackPlayer_Implementation(APlayerCharacter* Target) {
	Prey = Target;
	PlayAnimMontage(AttackAnim);
	Target->DieFrom(this);
}

void AMonsterCharacter::MulticastAttackCachette_Implementation(ACachette* Target) {
	Prey = Target;
	PlayAnimMontage(DestroyAnim);
	Target->DestroyCachette();
}

void AMonsterCharacter::MulticastStartHunt_Implementation(APlayerCharacter* Target) {
	Prey = Target;
	PlayAnimMontage(TauntAnim);
}