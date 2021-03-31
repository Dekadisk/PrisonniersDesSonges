#include "WanderBTTaskNode.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UWanderBTTaskNode::UWanderBTTaskNode()
{
	// Nous d�finissons le nom que protera le noeud dans le BT
	NodeName = "Wander";
}

/* Fonction d'ex�cution de la t�che, cette t�che devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UWanderBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->Wander();

	return EBTNodeResult::Succeeded;
}

/** Permet de d�finir une description pour la t�che. C'est ce texte qui
 appara�tra dans le noeud que nous ajouterons au Behavior Tree */
FString UWanderBTTaskNode::GetStaticDescription() const
{
	return TEXT("Mouvement non optimise.");
}