// Fill out your copyright notice in the Description page of Project Settings.


#include "DeathSpectatorPawn.h"

ADeathSpectatorPawn::ADeathSpectatorPawn() {
	bGenerateOverlapEventsDuringLevelStreaming = false;
	UPrimitiveComponent* ActorRootComponent = Cast<UPrimitiveComponent>(GetRootComponent());
	ActorRootComponent->SetGenerateOverlapEvents(false);
}