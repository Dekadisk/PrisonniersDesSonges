// Fill out your copyright notice in the Description page of Project Settings.


#include "TriggerBoxActor.h"
#include "Kismet/GameplayStatics.h"
#include "LabyrinthGameModeBase.h"
#include "PlayerCharacter.h"

ATriggerBoxActor::ATriggerBoxActor() {
	bDisableFocus = true;
	bReplicates = false;

	OverlapBox = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));

	OverlapBox->AttachTo(MeshComp);

	OverlapBox->OnComponentBeginOverlap.AddDynamic(this, &ATriggerBoxActor::OnOverlapBegin);
	OverlapBox->OnComponentEndOverlap.AddDynamic(this, &ATriggerBoxActor::OnOverlapEnd);
}

void ATriggerBoxActor::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if (HasAuthority() && player)
		CheckEvents(EPuzzleEventCheck::Overlap, player);
}

void ATriggerBoxActor::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	APlayerCharacter* player = Cast<APlayerCharacter>(OtherActor);
	if(HasAuthority() && player)
		CheckEvents(EPuzzleEventCheck::EndOverlap, player);
}