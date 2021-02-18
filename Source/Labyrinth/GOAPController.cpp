#include "GOAPController.h"

void AGOAPController::BeginPlay()
{
	// Loads actions.
	for (auto i = 0; i < actions.Num(); ++i)
	{
		if (actions[i] != NULL)
			auxActions.Push(actions[i].GetDefaultObject());
	}

	// Loads Current World.
	for (FState state : currentWorld)
		wsCurrentWorld.addState(state.name, state.value);

	// Loads Desired World.
	for (FState state : desiredWorld)
		wsDesiredWorld.addState(state.name, state.value);

	// Loads actions' preconditions and effects.
	for (UGOAPAction* a : auxActions)
		a->create_P_E();

	// Loads Planner.
	planner = new Planner(&wsCurrentWorld, &wsDesiredWorld, auxActions);
	planner->setMaxDepth(maxDepth);

	// Error messages.
	if (auxActions.Num() == 0)
		UE_LOG(LogTemp, Warning, TEXT("Actions not found in GOAPController."));

	if (wsCurrentWorld.isEmpty())
		UE_LOG(LogTemp, Warning, TEXT("Undefined Current World in GOAPController."));

	if (wsDesiredWorld.isEmpty())
		UE_LOG(LogTemp, Warning, TEXT("Undefined Desired World in GOAPController."));

	Super::BeginPlay();
}

bool AGOAPController::executeGOAP()
{
	if (generatePlan())
	{
		if (plan.Num() > 0)
		{
			// Gets next action to perform.
			UGOAPAction* aux = plan[plan.Num() - 1];

			// Performs an action and when it's done its effects are applied, changing the current world state.
			if (aux->doAction(GetPawn()))
				wsCurrentWorld.joinWorldState(aux->getEffects());

			return true;
		}
	}

	return false;
}

bool AGOAPController::generatePlan()
{
	if (auxActions.Num() > 0 && !wsCurrentWorld.isEmpty() && !wsDesiredWorld.isEmpty())
	{
		// Creates the cheapest plan of actions.
		plan = planner->generatePlan(GetPawn());

		return true;
	}

	return false;
}

TArray<UGOAPAction*> AGOAPController::getPlan()
{
	return plan;
}

void AGOAPController::setGoal(const TArray<FState>& newGoal)
{
	wsDesiredWorld.cleanStates();
	updateGoal(newGoal);
}

void AGOAPController::updateGoal(const TArray<FState>& states)
{
	for (FState state : states)
		wsDesiredWorld.addState(state.name, state.value);
}

void AGOAPController::setCurrentWorld(const TArray<FState>& newCurrentWorld)
{
	wsCurrentWorld.cleanStates();
	updateCurrentWorld(newCurrentWorld);
}

void AGOAPController::updateCurrentWorld(const TArray<FState>& states)
{
	for (FState state : states)
		wsCurrentWorld.addState(state.name, state.value);
}

TArray<FState> AGOAPController::getCurrentWorldStateStates()
{
	TArray<FState> worldStateStates;
	for (auto state : wsCurrentWorld.getStates())
	{
		worldStateStates.Add({ state.first, state.second });
	}

	return worldStateStates;
}

TArray<FState> AGOAPController::getDesiredWorldStateStates()
{
	TArray<FState> worldStateStates;
	for (auto state : wsDesiredWorld.getStates())
	{
		worldStateStates.Add({ state.first, state.second });
	}

	return worldStateStates;
}
