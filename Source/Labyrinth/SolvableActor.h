#pragma once

#include "CoreMinimal.h"
#include "UsableActor.h"
#include "PuzzleEventsData.h"
#include "SolvableActor.generated.h"

UCLASS(ABSTRACT)
class LABYRINTH_API ASolvableActor : public AUsableActor
{
	GENERATED_BODY()
	
public:	

	// Sets default values for this actor's properties
	ASolvableActor();

public:	

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, ReplicatedUsing = OnRep_Solvable)
	bool isSolved;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	bool hasErasedHint;

	UPROPERTY(VisibleAnywhere)
	TArray<class AUsableActor*> Elements;

	UFUNCTION()
	virtual void OnRep_Solvable() {}

	//Multi
	void GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const override;
};
