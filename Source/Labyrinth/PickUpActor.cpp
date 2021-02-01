// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUpActor.h"


// Called every frame
void APickUpActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

APickUpActor::APickUpActor()
{
	// Tous les objets qui peuvent �tre ramass�s son simul�s 
	// et peuvent �tre diss�min�s sur tout le niveau 
	MeshComp->SetSimulatePhysics(true);
	bReplicates = true;
	//bReplicateMovement = true;
	SetReplicateMovement(true);
}

void APickUpActor::BeginPlay()
{
	Super::BeginPlay();
	// Pourra �tre utilis� pour initialiser des valeurs 
}

void APickUpActor::OnUsed(AActor* InstigatorActor)
{
	Super::OnUsed(InstigatorActor);
	UGameplayStatics::PlaySoundAtLocation(this, PickupSound, GetActorLocation());

	Destroy();
}

