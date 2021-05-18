#include "LanternPickUpActor.h"
#include "LabCharacter.h"
#include "LabyrinthPlayerController.h"


ALanternPickUpActor::ALanternPickUpActor()
{
	LanternMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LanternMesh"));
	LanternMesh->SetupAttachment(MeshComp);
	bReplicates = true;

	static ConstructorHelpers::FClassFinder<ALanternHeld> LanternHeld_BP_F(TEXT("/Game/Blueprints/LanternHeld_BP"));
	LanternHeld_BP = LanternHeld_BP_F.Class;
}

void ALanternPickUpActor::OnEndFocus()
{
	Super::OnEndFocus();
	if (!bDisableFocus)
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage� 
		LanternMesh->SetRenderCustomDepth(false);
}

void ALanternPickUpActor::OnBeginFocus() {
	Super::OnBeginFocus();
	if (!bDisableFocus)
		// Utilis� par notre PostProcess pour le rendu d'un �surlignage�
		LanternMesh->SetRenderCustomDepth(true);
}

AActor* ALanternPickUpActor::SpawnHeld_BP()
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	return World->SpawnActor<AActor>(LanternHeld_BP,
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
			ALanternHeld* LanternHeld = Cast<ALanternHeld>(SpawnHeld_BP());
			LanternHeld->SetActorEnableCollision(false);
			
			// TODO : Should the player know what item instances it carries?
			playerController->bHasLantern = true;
			playerController->pLantern = LanternHeld;
			
			bool res = LanternHeld->GetLanternMesh()->AttachToComponent(player->GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, FName("thigh_r_lantern"));
			//LanternHeld_BP->ActivatePhysics();
			// Only used for debug.
			/*if (res)
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Lanterne dans la main")));
			else
				GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, FString::Printf(TEXT("Lanterne PAS dans la main")));*/
		}
	}


}

