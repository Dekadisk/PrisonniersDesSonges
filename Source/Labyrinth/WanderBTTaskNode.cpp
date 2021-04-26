#include "WanderBTTaskNode.h"
#include "AIEnemyController.h"

UWanderBTTaskNode::UWanderBTTaskNode()
{
	NodeName = "Wander";
}

EBTNodeResult::Type UWanderBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*)
{
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->Wander();

	return EBTNodeResult::Succeeded;
}

/** Permet de définir une description pour la tâche. C'est ce texte qui
 apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
FString UWanderBTTaskNode::GetStaticDescription() const
{
	return TEXT("Mouvement non optimise.");
}