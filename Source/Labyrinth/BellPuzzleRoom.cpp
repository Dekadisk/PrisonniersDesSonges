#include "BellPuzzleRoom.h"
#include <algorithm>
#include "Engine/StaticMeshSocket.h"
#include "BellPuzzleActor.h"
#include <Runtime\Engine\Classes\Kismet\GameplayStatics.h>
ABellPuzzleRoom::ABellPuzzleRoom() :nbBells {4}{
	
}

void ABellPuzzleRoom::InitPuzzle(FRandomStream seed) {

	Super::InitPuzzle(seed);
	stoneDoorActor = Cast<ABellDoorSolvableActor>(InstanceBP(TEXT("/Game/Blueprints/BellDoorSolvableActor_BP.BellDoorSolvableActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	stoneDoorActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Herse0"));

	TArray<int32> sonsIds{ 0,1,2,3,4,5,6 };
	stoneDoorActor->waited.Empty();
	for (int i = 0; i < nbBells; ++i) {
		int32 index = seed.GetUnsignedInt() % sonsIds.Num();
		int32 nextSound = sonsIds[index];
		sonsIds.RemoveAt(index);
		sons.Add(nextSound);
		stoneDoorActor->waited.Add(nextSound);
	}
}

void ABellPuzzleRoom::CreateBells(std::vector<LabBlock*> bells, LabBlock* bellHintPos, const TArray<ATile*>& tiles){
	std::for_each(begin(bells), end(bells),
		[&,counter = 0](LabBlock* labBlock)mutable
		{
			const UStaticMeshSocket* bellSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("BellHint0");
			if (bellSocket) {
				FTransform transform;

				bellSocket->GetSocketTransform(transform, tiles[labBlock->GetIndex()]->mesh);
				AActor* actor = InstanceBell(TEXT("/Game/Blueprints/BellPuzzleActor_BP.BellPuzzleActor_BP")
					, transform.GetLocation(), transform.GetRotation().Rotator());
				ABellPuzzleActor* bell = Cast<ABellPuzzleActor>(actor);
				bell->note = sons[counter++];
				//LINK DE LA BELL A LA PORTE
				FPE_ActorInteractions ai;
				ai.Actors.Add(stoneDoorActor);
				ai.Interaction = EPuzzleEventInteraction::Ring;
				FPE_Environment ev;
				ev.ActorInteractions.Add(ai);
				FPE_PuzzleEvent pe;
				pe.Environment = ev;
				FPE_PuzzleEventMaster em;
				em.OnlyOnce = false;
				em.Event = pe;
				em.Trigger = EPuzzleEventCheck::On;
				bell->PuzzleEvents.Add(em);

				//actor->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Bell0"));
			}
		});
	const UStaticMeshSocket* bellHintSocket = tiles[bellHintPos->GetIndex()]->mesh->GetSocketByName("BellHint0");
	if (bellHintSocket) {
		FTransform transform;

		bellHintSocket->GetSocketTransform(transform, tiles[bellHintPos->GetIndex()]->mesh);
		AActor* actor = InstanceBP(TEXT("/Game/Blueprints/BellHintActor_BP.BellHintActor_BP")
			, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
		ABellHintActor* bellHint = Cast<ABellHintActor>(actor);
		bellHint->waited.Empty();

		bellHint->waited = stoneDoorActor->waited;

		//actor->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Bell0"));
	}
}

AActor* ABellPuzzleRoom::InstanceBell(const TCHAR* bpName, FVector location, FRotator rotation, FVector scale)
{
	UObject* SpawnActor = Cast<UObject>(StaticLoadObject(UObject::StaticClass(), NULL, bpName));

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
	AActor* actor = World->SpawnActorDeferred<AActor>(GeneratedBP->GeneratedClass, FTransform{
			rotation,
			location });
	if (actor) {
		ABellPuzzleActor* bell = Cast<ABellPuzzleActor>(actor);
		bell->note = 1;
		bell->UpdateScale();
		UGameplayStatics::FinishSpawningActor(actor, FTransform{ rotation, location });
	}
	return actor;
}