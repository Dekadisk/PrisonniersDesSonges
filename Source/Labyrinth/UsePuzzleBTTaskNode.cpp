#include "UsePuzzleBTTaskNode.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UUsePuzzleBTTaskNode::UUsePuzzleBTTaskNode()
{
	// Nous définissons le nom que protera le noeud dans le BT
	NodeName = "UsePuzzle";
}

/* Fonction d'exécution de la tâche, cette tâche devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UUsePuzzleBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->UsePuzzle();

	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}

/** Permet de définir une description pour la tâche. C'est ce texte qui
 apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
FString UUsePuzzleBTTaskNode::GetStaticDescription() const
{
	return TEXT("Sabotage des puzzles.");
}