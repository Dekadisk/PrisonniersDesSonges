#include "UsePuzzleBTTaskNode.h"
#include "AIEnemyController.h"

UUsePuzzleBTTaskNode::UUsePuzzleBTTaskNode()
{
	NodeName = "UsePuzzle";
}

EBTNodeResult::Type UUsePuzzleBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->UsePuzzle();

	return EBTNodeResult::Succeeded;
}

FString UUsePuzzleBTTaskNode::GetStaticDescription() const
{
	return TEXT("Sabotage des puzzles.");
}