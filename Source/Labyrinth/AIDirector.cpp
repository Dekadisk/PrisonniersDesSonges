// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDirector.h"
#include "Kismet/GameplayStatics.h"
#include "AIEnemyController.h"
#include "SolvableActor.h"
#include "LabyrinthGameModeBase.h"

// Sets default values
AAIDirector::AAIDirector()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bNetLoadOnClient = false;

	SetActorTickInterval(1);
}

// Called when the game starts or when spawned
void AAIDirector::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> MonstersArray;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AAIEnemyController::StaticClass(), MonstersArray);
	if (MonstersArray.Num() > 0) {
		Monster = Cast<AAIEnemyController>(MonstersArray[0]);
	}

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASolvableActor::StaticClass(), Solvables);

}

// Called every frame
void AAIDirector::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Cast<ALabyrinthGameModeBase>(GetWorld()->GetAuthGameMode())->debug)
	{
		DebugDisplayInfo();
	}

}

void AAIDirector::AddPlayer(AActor* actor) {
	Players.Add(actor);
}

void AAIDirector::DebugDisplayInfo() {
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(0, 1.1f, FColor::Red, FString::Printf(TEXT("Nous avons %d enigmes. Ca va chauffer le cerveau L O L !"), Solvables.Num()));
		GEngine->AddOnScreenDebugMessage(1, 1.1f, FColor::Red, FString::Printf(TEXT("Nous avons %d joueurs. C'est incroyable tout ca lo !"), Players.Num()));
	}
}