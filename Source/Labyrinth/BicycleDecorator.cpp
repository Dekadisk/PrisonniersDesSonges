// Fill out your copyright notice in the Description page of Project Settings.


#include "BicycleDecorator.h"


// Sets default values
ABicycleDecorator::ABicycleDecorator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR1(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_bicycle.SM_bicycle'"));
	// check if path is valid
	if (MeshR1.Succeeded())
		M1 = MeshR1.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BicycleMesh"));

	SetRootComponent(mesh);
	kind = 0;
}

// Called when the game starts or when spawned
void ABicycleDecorator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ABicycleDecorator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABicycleDecorator::UpdateMesh()
{
	//kind is between 0 and 1
	switch (kind % 1) {
	case 0:
		mesh->SetStaticMesh(M1);
		break;
	}
}

void ABicycleDecorator::OnRep_UpdateMesh()
{
}
