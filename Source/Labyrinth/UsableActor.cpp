#include "UsableActor.h"
#include "SolvableActor.h"
#include <Runtime/AIModule/Classes/Perception/AIPerceptionSystem.h>
#include <Runtime/AIModule/Classes/Perception/AISense_Sight.h>

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

void AUsableActor::BeginPlay() {
	Super::BeginPlay();

	UAIPerceptionSystem::RegisterPerceptionStimuliSource(this, UAISense_Sight::StaticClass(), this);

	for (FPE_PuzzleEventMaster& pem : PuzzleEvents)
	{
		for (FPE_ActorInteractions& ai : pem.Event.Environment.ActorInteractions)
		{
			for (AUsableActor* a : ai.Actors) {
				if (ASolvableActor* sa = Cast<ASolvableActor>(a))
					if(!sa->Elements.Contains(this))
						sa->Elements.Add(this);
			}
		}
	}
}

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


