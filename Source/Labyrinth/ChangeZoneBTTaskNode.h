#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "ChangeZoneBTTaskNode.generated.h"

UCLASS()
class LABYRINTH_API UChangeZoneBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UChangeZoneBTTaskNode();

	/* Sera appel�e au d�marrage de la t�che et devra retourner Succeeded, Failed ou InProgress */
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	/* Sera appel�e constamment tant que la t�che n'est pas finie
	 (tant que ExecuteTask retourne InProgress) */
	virtual void TickTask(class UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

	/** Retourne une chaine de description pour la t�che. Ce texte appara�tre dans le BT */
	virtual FString GetStaticDescription() const override;
};
