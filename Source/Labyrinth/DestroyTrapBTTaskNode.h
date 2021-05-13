#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DestroyTrapBTTaskNode.generated.h"

UCLASS()
class LABYRINTH_API UDestroyTrapBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UDestroyTrapBTTaskNode();

	/* Sera appelée au démarrage de la tâche et devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** Retourne une chaine de description pour la tâche. Ce texte apparaître dans le BT */
	virtual FString GetStaticDescription() const override;
};
