#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "ClearBlackboardBTService.generated.h"

UCLASS()
class LABYRINTH_API UClearBlackboardBTService : public UBTService
{
	GENERATED_BODY()
	
public:
	UClearBlackboardBTService();

	/** Sera appelé à chaque «update» du service */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
};
