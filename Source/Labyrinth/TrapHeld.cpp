// Fill out your copyright notice in the Description page of Project Settings.


#include "TrapHeld.h"
#include <Labyrinth\LabCharacter.h>

// Called when the game starts or when spawned
void ATrapHeld::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATrapHeld::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Sets default values
ATrapHeld::ATrapHeld()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Lantern"));
	bReplicates = true;
	bNetUseOwnerRelevancy = true;

}

void ATrapHeld::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ATrapHeld, MyPawn);
}

void ATrapHeld::OnEnterInventory(ALabCharacter* NewOwner)
{
	SetOwningPawn(NewOwner);
	AttachMeshToPawn("thigh_l_trap");
}

void ATrapHeld::SetOwningPawn(ALabCharacter* NewOwner)
{
	if (MyPawn != NewOwner)
	{
		SetInstigator(NewOwner);
		MyPawn = NewOwner;
		// Propriétaire réseau pour les appels RPC.
		SetOwner(NewOwner);
	}
}

void ATrapHeld::AttachMeshToPawn(FName slot)
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

void ATrapHeld::DetachMeshFromPawn()
{
	Mesh->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
	Mesh->SetHiddenInGame(true);
}

void ATrapHeld::OnLeaveInventory()
{
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

UStaticMeshComponent* ATrapHeld::GetLanternMesh() const
{
	return Mesh;
}

void ATrapHeld::OnRep_MyPawn()
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