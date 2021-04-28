#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CheckPuzzlesBTTaskNode.generated.h"

UCLASS()
class LABYRINTH_API UCheckPuzzlesBTTaskNode : public UBTTaskNode
{
	GENERATED_BODY()
	
public:

	UCheckPuzzlesBTTaskNode();

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual FString GetStaticDescription() const override;
};
