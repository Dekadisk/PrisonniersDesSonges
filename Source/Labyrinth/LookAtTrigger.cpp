// Fill out your copyright notice in the Description page of Project Settings.


#include "LookAtTrigger.h"

ALookAtTrigger::ALookAtTrigger() {
	bDisableFocus = true;
	bReplicates = false;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));

	OverlapBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapBox->SetCollisionResponseToAllChannels(ECR_Ignore);
	OverlapBox->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Overlap);
	OverlapBox->AttachTo(MeshComp);
}
void ALookAtTrigger::Looking(APlayerCharacter* player) {
	if (timers.Contains(player))
		timers[player] += 0.1;
	else
		timers.Add(player, 0.1);
	if (timers[player] > lookingDuration && !alreadyCalled.Contains(player)) {
		CheckEvents(EPuzzleEventCheck::LookAt, player);
		alreadyCalled.Add(player);
	}
}

void ALookAtTrigger::ResetLooking(APlayerCharacter* player) {
	timers.Remove(player);
	alreadyCalled.Remove(player);
}
