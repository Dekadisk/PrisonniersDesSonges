#include "DestroyTrapBTTaskNode.h"
#include "AIEnemyController.h"

UDestroyTrapBTTaskNode::UDestroyTrapBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "DestroyTrap";

}

/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UDestroyTrapBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->DestroyTrap();

	return EBTNodeResult::Succeeded;
}

/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */
FString UDestroyTrapBTTaskNode::GetStaticDescription() const
{
	return TEXT("Detruire le piege.");
}
