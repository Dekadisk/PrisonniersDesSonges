#pragma once

#include "Planner.h"
#include "CoreMinimal.h"
#include "AIController.h"
#include "GOAPController.generated.h"

UCLASS()
class LABYRINTH_API AGOAPController : public AAIController
{
	GENERATED_BODY()

public:

	// State of the current world.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GOAP)
		TArray<FState> currentWorld;

	// State of the world in witch the goal has been achieved.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GOAP)
		TArray<FState> desiredWorld;

	// List of actions AI can do.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GOAP)
		TArray<TSubclassOf<UGOAPAction>> actions;

	// Maximum algorithm depth.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = GOAP)
		int maxDepth = 100;

private:

	Planner* planner;

	TArray<UGOAPAction*> auxActions;

	TArray<UGOAPAction*> plan;

	WorldState wsCurrentWorld;

	WorldState wsDesiredWorld;

public:

	AGOAPController() = default;

	virtual void BeginPlay() override;

	// Creates and execute the plan.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		bool executeGOAP();

	// Creates actions plan
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		bool generatePlan();

	// Returns the actions that make up the plan.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		TArray<UGOAPAction*> getPlan();

	// Sets the goal of the AI, this function can also be used to change the goal.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		void setGoal(const TArray<FState>& newGoal);

	// Adds or modifies atoms from the desired world state. Atoms already existing in the desired world state are modified by new input values.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		void updateGoal(const TArray<FState>& states);

	// Sets the current world state of the AI, this function can also be used to change the current world state.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		void setCurrentWorld(const TArray<FState>& newCurrentWorld);

	// Adds or modifies atoms from the current world state. Atoms already existing in the current world state are modified by new input values.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		void updateCurrentWorld(const TArray<FState>& states);

	// Returns the current world state atoms.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		TArray<FState> getCurrentWorldStateStates();

	// Returns the desired world state atoms.
	UFUNCTION(BlueprintCallable, Category = GOAPController)
		TArray<FState> getDesiredWorldStateStates();
};
