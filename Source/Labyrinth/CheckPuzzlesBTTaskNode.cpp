#include "CheckPuzzlesBTTaskNode.h"
#include "AIEnemyController.h"

UCheckPuzzlesBTTaskNode::UCheckPuzzlesBTTaskNode()
{
	// Nous d�finissons le nom que protera le noeud dans le BT
	NodeName = "CheckPuzzlesToInvestigate";
}

/* Fonction d'ex�cution de la t�che, cette t�che devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UCheckPuzzlesBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->CheckPuzzlesToInvestigate();

	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}

/** Permet de d�finir une description pour la t�che. C'est ce texte qui
 appara�tra dans le noeud que nous ajouterons au Behavior Tree */
FString UCheckPuzzlesBTTaskNode::GetStaticDescription() const
{
	return TEXT("V�rification des puzzles.");
}