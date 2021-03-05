// Fill out your copyright notice in the Description page of Project Settings.


#include "UpdateNextTargetPointBTTaskNode.h"
#include "AIEnemyController.h"

/** Constructeur de la classe */
UUpdateNextTargetPointBTTaskNode::UUpdateNextTargetPointBTTaskNode()
{
	// Nous d�finissons le nom que protera le noeud dans le BT
	NodeName = "UpdateNextTargetPoint";
}
/* Fonction d'ex�cution de la t�che, cette t�che devra retourner Succeeded, Failed ou InProgress */
EBTNodeResult::Type UUpdateNextTargetPointBTTaskNode::ExecuteTask(UBehaviorTreeComponent& OwnerComp,
	uint8* NodeMemory)
{
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Appeler la fonctionUpdateNextTargetPoint qui contient la logique pour s�lectionner
	 // le prochain TargetPoint
	AIEnemyController->UpdateNextTargetPoint();
	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}
/** Permet de d�finir une description pour la t�che. C'est ce texte qui
 appara�tra dans le noeud que nous ajouterons au Behavior Tree */
FString UUpdateNextTargetPointBTTaskNode::GetStaticDescription() const
{
	return TEXT("Actualisation du point suivant dans le chemin du monstre.");
}