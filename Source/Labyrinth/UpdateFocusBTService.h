#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "UpdateFocusBTService.generated.h"

UCLASS()
class LABYRINTH_API UUpdateFocusBTService : public UBTService
{
	GENERATED_BODY()
	
public:

	UUpdateFocusBTService();

	/** Sera appelé à chaque «update» du service */
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory,
		float DeltaSeconds) override;
};
