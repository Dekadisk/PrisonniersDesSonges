// Fill out your copyright notice in the Description page of Project Settings.


#include "MonsterCharacter.h"
#include "GameFramework/PlayerStart.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
#include "AIEnemyController.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "Runtime/AIModule/Classes/BehaviorTree/BlackboardComponent.h"

// Sets default values
AMonsterCharacter::AMonsterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Vitesse = 0.1f;
	MaxUseDistance = 800;

	static ConstructorHelpers::FClassFinder<APawn> LabBP{ TEXT("/Game/Blueprints/LabCharacter_BP") };
	LabClassBP = LabBP.Class;
}

// Called when the game starts or when spawned
void AMonsterCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMonsterCharacter::NotifyHit(class UPrimitiveComponent* MyComp,
	AActor* Other,
	class UPrimitiveComponent* OtherComp,
	bool bSelfMoved,
	FVector HitLocation,
	FVector HitNormal,
	FVector NormalImpulse,
	const FHitResult& Hit) {

	if (HasAuthority())
	{
		ALabCharacter* CastedActor = Cast<ALabCharacter>(Other);
		if (CastedActor) {
			AController* savedController = CastedActor->GetController();
			CastedActor->Destroy();
			TArray<AActor*> array;
			UGameplayStatics::GetAllActorsOfClass(GetWorld(), APlayerStart::StaticClass(), array);

			APlayerStart* pStart = Cast<APlayerStart>(*array.begin());

			ALabCharacter* newSpawned = GetWorld()->SpawnActor<ALabCharacter>(LabClassBP, pStart->GetActorTransform());
			savedController->Possess(newSpawned);

			AAIEnemyController* enemyController = Cast<AAIEnemyController>(GetController());
			if (enemyController) {
				UBrainComponent* brain = enemyController->BrainComponent;
				UBlackboardComponent* blackboard = brain->GetBlackboardComponent();
				blackboard->SetValueAsObject("TargetActorToFollow", NULL);
			}
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Blue, TEXT("Le client a touche"));
	}
}

// Called every frame
void AMonsterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMonsterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

