// Fill out your copyright notice in the Description page of Project Settings.


#include "RockDecorator.h"
// Sets default values
ARockDecorator::ARockDecorator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR1(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Rocks/SM_LCave_P_Free_Rock_01.SM_LCave_P_Free_Rock_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR2(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Rocks/SM_LCave_P_Free_Rock_02.SM_LCave_P_Free_Rock_02'"));
	// check if path is valid
	if (MeshR1.Succeeded())
		R1 = MeshR1.Object;
	if (MeshR2.Succeeded())
		R2 = MeshR2.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RockMesh"));

	SetRootComponent(mesh);
	kind = 0;
}

// Called when the game starts or when spawned
void ARockDecorator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ARockDecorator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ARockDecorator::UpdateMesh()
{
	//kind is between 0 and 1
	switch (kind % 2) {
	case 0:
		mesh->SetStaticMesh(R1);
		break;
	case 1:
		mesh->SetStaticMesh(R2);
		break;
	}
}

void ARockDecorator::OnRep_UpdateMesh()
{
}



