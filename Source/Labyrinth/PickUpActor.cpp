// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"


// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APickUpActor::APickUpActor()
{
	// Tous les objets qui peuvent être ramassés son simulés 
	// et peuvent être disséminés sur tout le niveau 
	MeshComp->SetSimulatePhysics(true);
	bReplicates = true;
	//bReplicateMovement = true;
	SetReplicateMovement(true);
}

void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	// Pourra être utilisé pour initialiser des valeurs 
}

void APickUpActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Destroy();
}

