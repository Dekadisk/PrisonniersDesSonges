#include "UsePuzzleBTTaskNode.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UUsePuzzleBTTaskNode::UUsePuzzleBTTaskNode()
{
	// Nous d�finissons le nom que protera le noeud dans le BT
	NodeName = "UsePuzzle";
}

/* Fonction d'ex�cution de la t�che, cette t�che devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UUsePuzzleBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->UsePuzzle();

	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}

/** Permet de d�finir une description pour la t�che. C'est ce texte qui
 appara�tra dans le noeud que nous ajouterons au Behavior Tree */
FString UUsePuzzleBTTaskNode::GetStaticDescription() const
{
	return TEXT("Sabotage des puzzles.");
}