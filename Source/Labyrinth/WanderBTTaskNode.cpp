#include "WanderBTTaskNode.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UWanderBTTaskNode::UWanderBTTaskNode()
{
	// Nous définissons le nom que protera le noeud dans le BT
	NodeName = "Wander";
}

/* Fonction d'exécution de la tâche, cette tâche devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UWanderBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
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