// Fill out your copyright notice in the Description page of Project Settings.


#include "FrameDecorator.h"

// Sets default values
AFrameDecorator::AFrameDecorator()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR1(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_frame_1.SM_frame_1'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR2(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_frame_2.SM_frame_2'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR3(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_frame_3.SM_frame_3'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshR4(TEXT("StaticMesh'/Game/ChildrensRoom/Meshes/SM_frame_4.SM_frame_4'"));
	// check if path is valid
	if (MeshR1.Succeeded())
		M1 = MeshR1.Object;
	if (MeshR2.Succeeded())
		M2 = MeshR2.Object;
	if (MeshR3.Succeeded())
		M3 = MeshR3.Object;
	if (MeshR4.Succeeded())
		M4 = MeshR4.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("FrameMesh"));

	SetRootComponent(mesh);
	kind = 0;
}

// Called when the game starts or when spawned
void AFrameDecorator::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFrameDecorator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFrameDecorator::UpdateMesh()
{
	//kind is between 0 and 1
	switch (kind % 4) {
	case 0:
		mesh->SetStaticMesh(M1);
		break;
	case 1:
		mesh->SetStaticMesh(M2);
		break;
	case 2:
		mesh->SetStaticMesh(M3);
		break;
	case 3:
		mesh->SetStaticMesh(M4);
		break;
	}
}

void AFrameDecorator::OnRep_UpdateMesh()
{
}

