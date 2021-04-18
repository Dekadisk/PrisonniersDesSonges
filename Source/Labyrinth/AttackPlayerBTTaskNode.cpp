#include "AttackPlayerBTTaskNode.h"
#include "AIEnemyController.h"

UAttackPlayerBTTaskNode::UAttackPlayerBTTaskNode()
{
	// Le nom que prendra le noeud dans le BT
	NodeName = "AttackPlayer";

}
/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UAttackPlayerBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8*
	NodeMemory)
{
	// Obtenir un pointeur sur notre AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->AttackPlayer();

	return EBTNodeResult::Succeeded;
}

/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */
FString UAttackPlayerBTTaskNode::GetStaticDescription() const
{
	return TEXT("Attaque le joueur.");
}