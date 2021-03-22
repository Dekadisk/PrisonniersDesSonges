// Fill out your copyright notice in the Description page of Project Settings.


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
	// Obtenir un pointeur sur AIEnemyController
	AAIEnemyController* AIEnemyController = Cast<AAIEnemyController>(OwnerComp.GetOwner());
	// Appeler la fonctionUpdateNextTargetPoint qui contient la logique pour s�lectionner
	 // le prochain TargetPoint
	AIEnemyController->Wander();
	//Nous retournons Succeeded
	return EBTNodeResult::Succeeded;
}
/** Permet de d�finir une description pour la t�che. C'est ce texte qui
 appara�tra dans le noeud que nous ajouterons au Behavior Tree */
FString UWanderBTTaskNode::GetStaticDescription() const
{
	return TEXT("Mouvement non optimise.");
}