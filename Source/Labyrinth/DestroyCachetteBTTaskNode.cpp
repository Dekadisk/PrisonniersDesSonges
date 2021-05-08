#include "DestroyCachetteBTTaskNode.h"
#include "AIEnemyController.h"

UDestroyCachetteBTTaskNode::UDestroyCachetteBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "DestroyCachette";

}

/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UDestroyCachetteBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->DestroyCachette();

	return EBTNodeResult::Succeeded;
}

/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
FString UDestroyCachetteBTTaskNode::GetStaticDescription() const
{
	return TEXT("Detruire la cachette.");
}