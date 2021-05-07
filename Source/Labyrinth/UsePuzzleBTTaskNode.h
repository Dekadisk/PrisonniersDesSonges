#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "UsePuzzleBTTaskNode.generated.h"

UCLASS()
class LABYRINTH_API UUsePuzzleBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UUsePuzzleBTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
