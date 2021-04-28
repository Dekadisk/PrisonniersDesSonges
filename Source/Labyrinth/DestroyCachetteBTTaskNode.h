#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "DestroyCachetteBTTaskNode.generated.h"

UCLASS()
class LABYRINTH_API UDestroyCachetteBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UDestroyCachetteBTTaskNode();

	/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */
	virtual FString GetStaticDescription() const override;
};
