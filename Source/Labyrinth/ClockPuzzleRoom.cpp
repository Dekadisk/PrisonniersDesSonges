#include "ClockPuzzleRoom.h"

#include "ClockPuzzleActor.h"
#include "LeverPuzzleActor.h"
#include "ClockDoorSolvableActor.h"
#include <algorithm>

#include "Engine/StaticMeshSocket.h"

AClockPuzzleRoom::AClockPuzzleRoom() {
	nbClocks = 4;
	solutions.Add(HandDir::UP);
	solutions.Add(HandDir::RIGHT);
	solutions.Add(HandDir::DOWN);
	solutions.Add(HandDir::LEFT);

	//DECORATIONS
	//ROCKS
	int counter = 0;
	TArray<FName> socketNames = mesh->GetAllSocketNames();
	for (FName name : socketNames) {
		if (name.ToString().Contains("Rock")) {
			UStaticMeshComponent* rock = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Rock") + FString::FromInt(counter++)));
			rock->SetupAttachment(mesh, name);
			rock->SetStaticMesh(Rock);
		}
		if (name.ToString().Contains("Mite")) {
			UStaticMeshComponent* mite = CreateDefaultSubobject<UStaticMeshComponent>(FName(TEXT("Mite") + FString::FromInt(counter++)));
			mite->SetupAttachment(mesh, name);
			mite->SetStaticMesh(Mite);
		}
	}
}

void AClockPuzzleRoom::InitPuzzle(FRandomStream seed)
{
	Super::InitPuzzle(seed);
	// Add Lever and Herse
	leverActor = Cast<ALeverPuzzleActor>(InstanceBP(TEXT("/Game/Blueprints/LeverPuzzleActor_BP.LeverPuzzleActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	herseActor = Cast<AClockDoorSolvableActor>(InstanceBP(TEXT("/Game/Blueprints/ClockDoorSolvableActor_BP.ClockDoorSolvableActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	leverActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Lever0"));
	herseActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Herse0"));

	herseActor->unlockLimit = nbClocks;
	//Add clocks
	solutions.Empty();
	randomSeed = seed;

	FPE_ActorInteractions ailever;
	ailever.Actors.Add(herseActor);
	ailever.Interaction = EPuzzleEventInteraction::Activate;
	FPE_Environment evlever;
	evlever.ActorInteractions.Add(ailever);
	FPE_PuzzleEvent pelever;
	pelever.Environment = evlever;
	FPE_PuzzleEventMaster emlever;
	emlever.Event = pelever;
	emlever.Trigger = EPuzzleEventCheck::On;
	emlever.OnlyOnce = false;
	leverActor->PuzzleEvents.Add(emlever);

	for (int clockId = 0; clockId < nbClocks; clockId++)
		solutions.Add(HandDir(seed.GetUnsignedInt() % 8));
}

void AClockPuzzleRoom::CreateClocks(std::vector<LabBlock*> clocksPos, const TArray<ATile*>& tiles)
{
	std::for_each(begin(clocksPos), end(clocksPos),
		[&, counter = 0](LabBlock* labBlock)mutable
	{
		const UStaticMeshSocket* clockSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Clock0");
		if (clockSocket) {
			FTransform transform;

			clockSocket->GetSocketTransform(transform, tiles[labBlock->GetIndex()]->mesh);
			AClockPuzzleActor* clock = Cast<AClockPuzzleActor>(InstanceBP(TEXT("/Game/Blueprints/ClockPuzzleActor_BP.ClockPuzzleActor_BP")
				, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D()));
			clock->clockNumber = counter;
			clock->OnRep_UpdateDecalMaterial();

			FPE_ActorInteractions ai;
			ai.Actors.Add(herseActor);
			ai.Interaction = EPuzzleEventInteraction::Unlock;
			FPE_Environment ev;
			ev.ActorInteractions.Add(ai);
			FPE_PuzzleEvent pe;
			pe.Environment = ev;
			FPE_PuzzleEventMaster em;
			em.OnlyOnce = false;
			em.Event = pe;
			em.Trigger = EPuzzleEventCheck::Unlock;
			clock->PuzzleEvents.Add(em);

			FPE_ActorInteractions ai2;
			ai2.Actors.Add(herseActor);
			ai2.Interaction = EPuzzleEventInteraction::Lock;
			FPE_Environment ev2;
			ev2.ActorInteractions.Add(ai2);
			FPE_PuzzleEvent pe2;
			pe2.Environment = ev2;
			FPE_PuzzleEventMaster em2;
			em2.OnlyOnce = false;
			em2.Event = pe2;
			em2.Trigger = EPuzzleEventCheck::Lock;
			clock->PuzzleEvents.Add(em2);

			clock->unlockPos = int(solutions[counter]);
			do { clock->startPos = randomSeed.GetUnsignedInt() % 8; } while (clock->startPos == clock->unlockPos);
			clock->currPos = clock->startPos;
			clock->OnConstruction({});

			counter++;
		}
	});
}
