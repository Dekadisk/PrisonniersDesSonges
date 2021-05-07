// Fill out your copyright notice in the Description page of Project Settings.


#include "TrumpetDecorator.h"

// Sets default values
ATrumpetDecorator::ATrumpetDecorator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR1(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_trumpet.SM_trumpet'"));
	// check if path is valid
	if (MeshR1.Succeeded())
		M1 = MeshR1.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TrumpetMesh"));

	SetRootComponent(mesh);
	kind = 0;
}

// Called when the game starts or when spawned
void ATrumpetDecorator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ATrumpetDecorator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ATrumpetDecorator::UpdateMesh()
{
	//kind is between 0 and 1
	switch (kind % 1) {
	case 0:
		mesh->SetStaticMesh(M1);
		break;
	}
}

void ATrumpetDecorator::OnRep_UpdateMesh()
{
}

