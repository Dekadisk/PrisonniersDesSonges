// Fill out your copyright notice in the Description page of Project Settings.


#include "LanternHeld.h"
#include "LabyrinthPlayerController.h"

// Sets default values
ALanternHeld::ALanternHeld()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Lantern"));
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

}

void ALanternHeld::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ALanternHeld, MyPawn);
}

// Called when the game starts or when spawned
void ALanternHeld::BeginPlay()
{
	Super::BeginPlay();
	
}

void ALanternHeld::OnEnterInventory(ALabCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
	AttachMeshToPawn("hand_r_lantern");
	Cast<ALabyrinthPlayerController>(NewOwner->GetController())->PlayMusic(SonLanterne);
}

// Called every frame
void ALanternHeld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALanternHeld::SetOwningPawn(ALabCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		SetInstigator(NewOwner);
		MyPawn = NewOwner;
		// Propriétaire réseau pour les appels RPC.
		SetOwner(NewOwner);
	}
}

void ALanternHeld::AttachMeshToPawn(FName slot)
{
	if (MyPawn)
	{
		// Retirer et cacher
		DetachMeshFromPawn();
		USkeletalMeshComponent* PawnMesh = MyPawn->GetMesh();
		FName AttachPoint = slot;
		Mesh->SetHiddenInGame(false); // On le rend visible
		Mesh->AttachToComponent(PawnMesh, FAttachmentTransformRules::SnapToTargetIncludingScale,
			AttachPoint);
	}
}

void ALanternHeld::DetachMeshFromPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetHiddenInGame(true);
}

void ALanternHeld::OnLeaveInventory()
{
	Cast<ALabyrinthPlayerController>(MyPawn->GetController())->PlayMusic(SonLanterneDrop);
	if (HasAuthority())
	{
		SetOwningPawn(nullptr);
	}
	/*if (IsEquippedOrPending())
	{
		OnUnEquip();
	}*/
	DetachMeshFromPawn();
}

USkeletalMeshComponent* ALanternHeld::GetLanternMesh() const
{
	return Mesh;
}

void ALanternHeld::OnRep_MyPawn()
{
	if (MyPawn)
	{
		OnEnterInventory(MyPawn);
	}
	else
	{
		OnLeaveInventory();
	}
}