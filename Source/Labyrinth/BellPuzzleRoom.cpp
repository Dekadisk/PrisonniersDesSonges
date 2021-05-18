#include "BellPuzzleRoom.h"
#include <algorithm>
#include "Engine/StaticMeshSocket.h"
#include "Kismet\GameplayStatics.h"

ABellPuzzleRoom::ABellPuzzleRoom() :nbBells {4}{
	static ConstructorHelpers::FClassFinder<ABellDoorSolvableActor> BellDoorSolvableActor_BP_F(TEXT("/Game/Blueprints/BellDoorSolvableActor_BP"));
	static ConstructorHelpers::FClassFinder<ABellHintActor> BellHintActor_BP_F(TEXT("/Game/Blueprints/BellHintActor_BP"));
	static ConstructorHelpers::FClassFinder<ABellPuzzleActor> BellPuzzleActor_BP_F(TEXT("/Game/Blueprints/BellPuzzleActor_BP"));
	BellDoorSolvableActor_BP = BellDoorSolvableActor_BP_F.Class;
	BellHintActor_BP = BellHintActor_BP_F.Class;
	BellPuzzleActor_BP = BellPuzzleActor_BP_F.Class;

}

void ABellPuzzleRoom::InitPuzzle(FRandomStream seed, PuzzleDifficulty _difficulty) {

	Super::InitPuzzle(seed, _difficulty);

	switch (difficulty)
	{
	case PuzzleDifficulty::Easy:
		nbBells = 2;
		break;
	case PuzzleDifficulty::Medium:
		nbBells = 3;
		break;
	case PuzzleDifficulty::Hard:
		nbBells = 4;
		break;
	default:
		nbBells = 4;
		break;
	}
	stoneDoorActor = Cast<ABellDoorSolvableActor>(InstanceBP(BellDoorSolvableActor_BP,/*TEXT("/Game/Blueprints/BellDoorSolvableActor_BP.BellDoorSolvableActor_BP"),*/ FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
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
	stoneDoorActor->Tags.Add(Tags[0]);
}

void ABellPuzzleRoom::CreateBells(std::vector<LabBlock*> bells, LabBlock* bellHintPos, const TArray<ATile*>& tiles){
	std::for_each(begin(bells), end(bells),
		[&,counter = 0](LabBlock* labBlock)mutable
		{
			const UStaticMeshSocket* bellSocket = tiles[labBlock->GetIndex()]->mesh->GetSocketByName("Bell0");
			if (bellSocket) {
				FTransform transform;

				bellSocket->GetSocketTransform(transform, tiles[labBlock->GetIndex()]->mesh);
				AActor* actor = InstanceBell(/*TEXT("/Game/Blueprints/BellPuzzleActor_BP.BellPuzzleActor_BP")*/
					 transform.GetLocation(), transform.GetRotation().Rotator());
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
		AActor* actor = InstanceBP(BellHintActor_BP/*TEXT("/Game/Blueprints/BellHintActor_BP.BellHintActor_BP")*/
			, transform.GetLocation(), transform.GetRotation().Rotator(), transform.GetScale3D());
		ABellHintActor* bellHint = Cast<ABellHintActor>(actor);
		bellHint->waited.Empty();

		bellHint->waited = stoneDoorActor->waited;

		//actor->AttachToComponent(tiles[labBlock->GetIndex()]->mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Bell0"));
	}
}

AActor* ABellPuzzleRoom::InstanceBell(FVector location, FRotator rotation, FVector scale)
{
	UWorld* World = GetWorld();
	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	AActor* actor = World->SpawnActorDeferred<AActor>(BellPuzzleActor_BP, FTransform{
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