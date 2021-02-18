#pragma once

#include "WorldState.h"
#include "CoreMinimal.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/NoExportTypes.h"
#include "GOAPAction.generated.h"

USTRUCT(BlueprintType, Blueprintable)
struct FState
{
	GENERATED_USTRUCT_BODY()

		// Name of the atom.
		UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Atom)
		FString name;

	// Value of the atom.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Atom)
		bool value;

};

UCLASS()
class LABYRINTH_API UGOAPAction : public UObject
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		FString name;

	// Cost of the action. The planner will take this into account when making the cheapest plan.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		float cost;

	// Object or class type of actor this action's target should have. This can be None if your action doesn't need a target.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Properties)
		TSubclassOf<AActor> targetsType;

	// Conditions needed to perform the action.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldState)
		TArray<FState> preconditions;

	// Effects caused by the action.
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = WorldState)
		TArray<FState> effects;


private:

	AActor* target;

	WorldState wsPreconditions;

	WorldState wsEffects;

public:

	UGOAPAction() = default;

	// Search all actors of targetsType class located in the world.
	UFUNCTION(BlueprintCallable, Category = GOAPAction)
	TArray<AActor*> getTargetsList(APawn* owner);

	// Optional function to check if it's possible to perform the action.
	UFUNCTION()
	virtual bool checkProceduralPrecondition(APawn* owner);

	// Performs the action.
	UFUNCTION()
	virtual bool doAction(APawn* owner);

	// Generate action's preconditions and effects.
	void create_P_E();

	// COMPARATORS

	bool operator==(UGOAPAction& action);

	bool operator!=(UGOAPAction& action);

	// GETS

	FString getName();

	float getCost();

	// Gets the chosen target from targetList or the one specific in setTarget().
	UFUNCTION(BlueprintCallable, Category = GOAPAction)
		AActor* getTarget();

	WorldState getPreconditions();

	WorldState getEffects();

	// SETS

	void setName(FString name);

	void setCost(float cost);

	// Sets a specific target.
	UFUNCTION(BlueprintCallable, Category = GOAPAction)
		void setTarget(AActor* target);

	void setPreconditions(WorldState preconditions);

	void setEffects(WorldState effects);
};
