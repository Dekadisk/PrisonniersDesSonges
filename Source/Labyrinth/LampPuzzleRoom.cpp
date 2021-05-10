// Fill out your copyright notice in the Description page of Project Settings.


#include "LampPuzzleRoom.h"
#include <algorithm>
#include "Engine/StaticMeshSocket.h"
#include "Kismet\GameplayStatics.h"
#include <vector>
#include <algorithm>
ALampPuzzleRoom::ALampPuzzleRoom() :nbLampsPerGroup{4} {

}

void ALampPuzzleRoom::InitPuzzle(FRandomStream seed, PuzzleDifficulty _difficulty)
{
	Super::InitPuzzle(seed, _difficulty);
	solvableActor = Cast<ALampDoorSolvableActor>(InstanceBP(TEXT("/Game/Blueprints/LampDoorSolvableActor_BP.LampDoorSolvableActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	solvableActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Herse0"));
	
	switch (difficulty)
	{
	case PuzzleDifficulty::Easy:
		nbGroups = 1;
		break;
	case PuzzleDifficulty::Medium:
		nbGroups = 2;
		break;
	case PuzzleDifficulty::Hard:
		nbGroups = 3;
		break;
	default:
		nbGroups = 1;
		break;
	}
	solvableActor->unlockLimit = nbLampsPerGroup*nbGroups;
	randomSeed = seed;

	solvableActor->Tags.Add(Tags[0]);
}

void ALampPuzzleRoom::CreateLamps(std::vector<LabBlock*> lamps, const TArray<ATile*>& tiles)
{
	std::for_each(begin(lamps), end(lamps),
		[&, counter = 0](LabBlock* labBlock)mutable
	{
		const UStaticMeshSocket* lampSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Lamp0");
		if (lampSocket) {
			FTransform transform;

			lampSocket->GetSocketTransform(transform, tiles[labBlock->GetIndex()]->mesh);
			AActor* actor = InstanceBP(TEXT("/Game/Blueprints/LampPuzzleActor_BP.LampPuzzleActor_BP")
				, transform.GetLocation(), transform.GetRotation().Rotator(), {1.f,1.f,1.f});
			ALampPuzzleActor* lamp = Cast<ALampPuzzleActor>(actor);
			lampsActors.Add(lamp);
			lamp->Tags.Add(Tags[0]);
		}
	});
	for (int i = 0; i < lampsActors.Num(); ++i) {
		//Use
		FPE_ActorInteractions aiUse;
		aiUse.Actors.Add(lampsActors[(lampsActors.Num()+i-1)% lampsActors.Num()]);
		aiUse.Actors.Add(lampsActors[(lampsActors.Num()+i+1)% lampsActors.Num()]);
		aiUse.Interaction = EPuzzleEventInteraction::Use;
		FPE_Environment evUse;
		evUse.ActorInteractions.Add(aiUse);
		FPE_PuzzleEvent peUse;
		peUse.Environment = evUse;
		FPE_PuzzleEventMaster emUse;
		emUse.OnlyOnce = false;
		emUse.Event = peUse;
		emUse.Trigger = EPuzzleEventCheck::Use;
		lampsActors[i]->PuzzleEvents.Add(emUse);

		//ON
		FPE_ActorInteractions aiOn;
		aiOn.Actors.Add(solvableActor);
		aiOn.Interaction = EPuzzleEventInteraction::Unlock;
		FPE_Environment evOn;
		evOn.ActorInteractions.Add(aiOn);
		FPE_PuzzleEvent peOn;
		peOn.Environment = evOn;
		FPE_PuzzleEventMaster emOn;
		emOn.OnlyOnce = false;
		emOn.Event = peOn;
		emOn.Trigger = EPuzzleEventCheck::On;
		lampsActors[i]->PuzzleEvents.Add(emOn);

		//Off
		FPE_ActorInteractions aiOff;
		aiOff.Actors.Add(solvableActor);
		aiOff.Interaction = EPuzzleEventInteraction::Lock;
		FPE_Environment evOff;
		evOff.ActorInteractions.Add(aiOff);
		FPE_PuzzleEvent peOff;
		peOff.Environment = evOff;
		FPE_PuzzleEventMaster emOff;
		emOff.OnlyOnce = false;
		emOff.Event = peOff;
		emOff.Trigger = EPuzzleEventCheck::Off;
		lampsActors[i]->PuzzleEvents.Add(emOff);
	}

	int randomLightsOn = randomSeed.GetUnsignedInt() % 3;

	std::vector<int> chosenIndexes;
	int chosen;
	switch (randomLightsOn)
	{
	case 0:
		break;
	case 1:
		chosen = randomSeed.GetUnsignedInt() % lampsActors.Num();
		chosenIndexes.push_back(chosen);
		lampsActors[chosen]->Use(true, nullptr);
		do {
			chosen = randomSeed.GetUnsignedInt() % lampsActors.Num();
		} while (std::find(chosenIndexes.begin(), chosenIndexes.end(),chosen) != chosenIndexes.end());
		chosenIndexes.push_back(chosen);
		lampsActors[chosen]->Use(true, nullptr);
		break;
	case 2:
		chosen = randomSeed.GetUnsignedInt() % lampsActors.Num();
		chosenIndexes.push_back(chosen);
		lampsActors[chosen]->Use(true, nullptr);
		do {
			chosen = randomSeed.GetUnsignedInt() % lampsActors.Num();
		} while (std::find(chosenIndexes.begin(), chosenIndexes.end(), chosen) != chosenIndexes.end());
		chosenIndexes.push_back(chosen);
		lampsActors[chosen]->Use(true, nullptr);
		do {
			chosen = randomSeed.GetUnsignedInt() % lampsActors.Num();
		} while (std::find(chosenIndexes.begin(), chosenIndexes.end(), chosen) != chosenIndexes.end());
		lampsActors[chosen]->Use(true, nullptr);
		break;
	default:
		break;
	}
}
