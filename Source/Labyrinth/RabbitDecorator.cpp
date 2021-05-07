// Fill out your copyright notice in the Description page of Project Settings.


#include "RabbitDecorator.h"

// Sets default values
ARabbitDecorator::ARabbitDecorator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR1(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_toys_rabbit.SM_toys_rabbit'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR2(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_toys_bear.SM_toys_bear'"));
	// check if path is valid
	if (MeshR1.Succeeded())
		M1 = MeshR1.Object;
	if (MeshR2.Succeeded())
		M2 = MeshR2.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RabbitMesh"));

	SetRootComponent(mesh);
	kind = 0;
}

// Called when the game starts or when spawned
void ARabbitDecorator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARabbitDecorator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARabbitDecorator::UpdateMesh()
{
	//kind is between 0 and 1
	switch (kind % 2) {
	case 0:
		mesh->SetStaticMesh(M1);
		break;
	case 1:
		mesh->SetStaticMesh(M2);
		break;
	}
}

void ARabbitDecorator::OnRep_UpdateMesh()
{
}


