// Fill out your copyright notice in the Description page of Project Settings.


#include "LanternPickUpActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"
#include "LanternHeld.h"

ALanternPickUpActor::ALanternPickUpActor()
{
	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
	LanternMesh->SetupAttachment(MeshComp);
	bReplicates = true;
}

void ALanternPickUpActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage� 
		LanternMesh->SetRenderCustomDepth(false);
	}
}

void ALanternPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
	{
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		LanternMesh->SetRenderCustomDepth(true);

	}
}

AActor* ALanternPickUpActor::SpawnHeld_BP()
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, TEXT("/Game/Blueprints/LanternHeld_BP.LanternHeld_BP")));

	UBlueprint* GeneratedBP = Cast<UBlueprint>(SpawnActor);
	if (!SpawnActor)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CANT FIND OBJECT TO SPAWN")));
		return nullptr;
	}

	UClass* SpawnClass = SpawnActor->StaticClass();
	if (SpawnClass == NULL)
	{
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("CLASS == NULL")));
		return nullptr;
	}

	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<AActor>(GeneratedBP->GeneratedClass,
		FTransform{
			FRotator{0, 0, 0},
			FVector{0, 0, 0},
			FVector{0.1f, 0.1f, 0.1f}
		}, 
		SpawnParams);
}

void ALanternPickUpActor::Use(bool Event, APawn* InstigatorPawn)
{
	ALabCharacter* player = Cast<ALabCharacter>(InstigatorPawn);
	if (IsValid(player))
	{
		ALabyrinthPlayerController* playerController = Cast<ALabyrinthPlayerController>(player->GetController());

		if (IsValid(playerController) && !playerController->bHasLantern)
		{
			// Destroying the PickUpActor.
			Super::Use(Event,InstigatorPawn);
			
			// Setting up spawn parameters.
			FActorSpawnParameters SpawnInfo;
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			
			// Creating a new LanternHeld_BP.
			ALanternHeld* LanternHeld_BP = Cast<ALanternHeld>(SpawnHeld_BP());
			LanternHeld_BP->SetActorEnableCollision(false);
			
			// TODO : Should the player know what item instances it carries?
			playerController->bHasLantern = true;

			
			bool res = LanternHeld_BP->GetLanternMesh()->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("thigh_r_lantern"));
			//LanternHeld_BP->ActivatePhysics();
			// Only used for debug.
			/*if (res)
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Lanterne dans la main")));
			else
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Lanterne PAS dans la main")));*/
		}
	}


}

