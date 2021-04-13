#include "UsableActor.h"

AUsableActor::AUsableActor()
{
	PrimaryActorTick.bCanEverTick = true;
	bDisableFocus = false;
	bReplicates = true;

	static ConstructorHelpers::FClassFinder<AEventMaker> EventMakerObject(
		TEXT("/Game/Blueprints/EventMaker_BP"));

	if (EventMakerObject.Class != NULL)
	{
		EventMaker = EventMakerObject.Class;
	}

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
}

//void AUsableActor::Use(bool Event, APawn* InstigatorPawn = nullptr)
//{
//	// Rien ici, les classes dérivées s'en occuperont 
//}

void AUsableActor::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(AUsableActor, bDisableFocus);
}

void AUsableActor::OnBeginFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage»
		MeshComp->SetRenderCustomDepth(true);
	}
}

void AUsableActor::OnEndFocus()
{
	if (!bDisableFocus)
	{
		// Utilisé par notre PostProcess pour le rendu d'un «surlignage» 
		MeshComp->SetRenderCustomDepth(false);
	}
}


void AUsableActor::CheckEvents(EPuzzleEventCheck check, APawn* InstigatorPawn) {
	TArray<FPE_PuzzleEventMaster> toTrigger;
	for (FPE_PuzzleEventMaster& pem : PuzzleEvents)
	{
		if (pem.Trigger == check) {
			pem.Counter++;
			if (pem.Counter == pem.AfterNbTimes)
			{
				toTrigger.Add(pem);
				if (!pem.OnlyOnce)
				{
					pem.Counter = 0;
				}
			}
		}
	}

	for (FPE_PuzzleEventMaster& pem : toTrigger) {
		// SPAWN EVENTMAKER
		FActorSpawnParameters SpawnInfo;
		SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
		SpawnInfo.Owner = this;
		SpawnInfo.Instigator = InstigatorPawn;
		AEventMaker* Maker = GetWorld()->SpawnActor<AEventMaker>(EventMaker, FVector::ZeroVector, FRotator::ZeroRotator, SpawnInfo);
		Maker->ExecuteEvent(pem.Event);
	}
}


