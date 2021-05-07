#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "WanderBTTaskNode.generated.h"

UCLASS()
class LABYRINTH_API UWanderBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UWanderBTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** Permet de définir une description pour la tâche. C'est ce texte qui
	 apparaîtra dans le noeud que nous ajouterons au Behavior Tree */
	virtual FString GetStaticDescription() const override;
};
