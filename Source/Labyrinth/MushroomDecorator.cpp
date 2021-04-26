// Fill out your copyright notice in the Description page of Project Settings.


#include "MushroomDecorator.h"

// Sets default values
AMushroomDecorator::AMushroomDecorator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM1(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_01.SM_LProp_Free_LightMushroom_01'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM2(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_02.SM_LProp_Free_LightMushroom_02'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM3(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_03.SM_LProp_Free_LightMushroom_03'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM4(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_04.SM_LProp_Free_LightMushroom_04'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM5(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_05.SM_LProp_Free_LightMushroom_05'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM6(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_06.SM_LProp_Free_LightMushroom_06'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM7(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_07.SM_LProp_Free_LightMushroom_07'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM8(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_08.SM_LProp_Free_LightMushroom_08'"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshM9(TEXT("StaticMesh'/Game/LuosCaves/Meshes/Props_Mushrooms/SM_LProp_Free_LightMushroom_09.SM_LProp_Free_LightMushroom_09'"));
	
	// check if path is valid
	if (MeshM1.Succeeded())
		M1 = MeshM1.Object;
	if (MeshM2.Succeeded())
		M2 = MeshM2.Object;
	if (MeshM3.Succeeded())
		M3 = MeshM3.Object;
	if (MeshM4.Succeeded())
		M4 = MeshM4.Object;
	if (MeshM5.Succeeded())
		M5 = MeshM5.Object;
	if (MeshM6.Succeeded())
		M6 = MeshM6.Object;
	if (MeshM7.Succeeded())
		M7 = MeshM7.Object;
	if (MeshM8.Succeeded())
		M8 = MeshM8.Object;
	if (MeshM9.Succeeded())
		M9 = MeshM9.Object;

	mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MushroomMesh"));

	SetRootComponent(mesh);
	kind = 0;
}

// Called when the game starts or when spawned
void AMushroomDecorator::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMushroomDecorator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMushroomDecorator::UpdateMesh()
{
	//kind is between 0 and 8
	switch (kind%9) {
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
	case 4:
		mesh->SetStaticMesh(M5);
		break;
	case 5:
		mesh->SetStaticMesh(M6);
		break;
	case 6:
		mesh->SetStaticMesh(M7);
		break;
	case 7:
		mesh->SetStaticMesh(M8);
		break;
	case 8:
		mesh->SetStaticMesh(M9);
		break;
	}
}

void AMushroomDecorator::OnRep_UpdateMesh()
{
}

