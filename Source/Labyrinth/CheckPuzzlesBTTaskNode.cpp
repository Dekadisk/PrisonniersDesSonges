#include "CheckPuzzlesBTTaskNode.h"
#include "AIEnemyController.h"

UCheckPuzzlesBTTaskNode::UCheckPuzzlesBTTaskNode()
{
	// Nous définissons le nom que protera le noeud dans le BT
	NodeName = "CheckPuzzlesToInvestigate";
}

/* Fonction d'exécution de la tâche, cette tâche devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UCheckPuzzlesBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());

	AIEnemyController->CheckPuzzlesToInvestigate();

	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}

/** Permet de définir une description pour la tâche. C'est ce texte qui
 apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
FString UCheckPuzzlesBTTaskNode::GetStaticDescription() const
{
	return TEXT("Vérification des puzzles.");
}