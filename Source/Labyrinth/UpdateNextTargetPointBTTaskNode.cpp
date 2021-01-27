// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateNextTargetPointBTTaskNode.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UUpdateNextTargetPointBTTaskNode::UUpdateNextTargetPointBTTaskNode()
{
	// Nous définissons le nom que protera le noeud dans le BT
	NodeName = "UpdateNextTargetPoint";
}
/* Fonction d'exécution de la tâche, cette tâche devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UUpdateNextTargetPointBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Appeler la fonctionUpdateNextTargetPoint qui contient la logique pour sélectionner
	 // le prochain TargetPoint
	AIEnemyController->UpdateNextTargetPoint();
	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}
/** Permet de définir une description pour la tâche. C'est ce texte qui
 apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
FString UUpdateNextTargetPointBTTaskNode::GetStaticDescription() const
{
	return TEXT("Actualisation du point suivant dans le chemin du monstre.");
}