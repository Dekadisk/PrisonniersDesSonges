#include "BellPuzzleRoom.h"
#include "BellDoorSolvableActor.h"
ABellPuzzleRoom::ABellPuzzleRoom() :nbBells {4}{
	
}

void ABellPuzzleRoom::InitPuzzle(FRandomStream seed) {

	ABellDoorSolvableActor* stoneDoorActor = Cast<ABellDoorSolvableActor>(InstanceBP(TEXT("/Game/Blueprints/BellDoorSolvableActor_BP.BellDoorSolvableActor_BP"), FVector{ 0,0,0 }, FRotator::ZeroRotator, FVector{ 1,1,1 }));
	stoneDoorActor->AttachToComponent(mesh, FAttachmentTransformRules(EAttachmentRule::KeepRelative, false), TEXT("Herse0"));
}