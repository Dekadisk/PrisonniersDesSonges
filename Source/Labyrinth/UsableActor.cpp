// Fill out your copyright notice in the Description page of Project Settings.


#include "UsableActor.h"

// Sets default values
AUsableActor::AUsableActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bDisableFocus = false;


	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

// Called when the game starts or when spawned
void AUsableActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AUsableActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUsableActor::OnUsed(AActor* InstigatorActor)
{
	// Rien ici, les classes dérivées s'en occuperont 
}

void AUsableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUsableActor, bDisableFocus);
}

void AUsableActor::OnBeginFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshComp->SetRenderCustomDepth(true);
	}
	
}

void AUsableActor::OnEndFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		MeshComp->SetRenderCustomDepth(false);
	}
	
}

