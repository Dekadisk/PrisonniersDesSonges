#pragma once

#include "WorldState.h"
#include "GOAPNode.h"
#include "GOAPAction.h"
#include "CoreMinimal.h"

class Planner
{
private:

	WorldState* currentWorld;

	WorldState* goal;

	TArray<UGOAPAction*> actions;

	TArray<GOAPNode> openList;

	TArray<GOAPNode> closedList;

	int maxDepth;

public:

	Planner() = default;

	~Planner() = default;

	Planner(WorldState* currentws, WorldState* _goal, const TArray<UGOAPAction*>& _actions);

	// Get the node with lowest F's value. F = G + H.
	GOAPNode lowestFinList(const TArray<GOAPNode>& opList);

	// Returns the nodes adjacent to the current one.
	TArray<GOAPNode> getAdjacent(GOAPNode current, const TArray<UGOAPAction*>& vActions, APawn* p);

	// A* pathfinding algorithm.
	TArray<UGOAPAction*> generatePlan(APawn* p);

	void addAction(UGOAPAction* a);

	//GETS

	WorldState getGoal();

	WorldState getCurrentWorld();

	int getMaxDepth();

	//SETS

	void setGoal(WorldState* _goal);

	void setCurrentWorld(WorldState* currentws);

	void setMaxDepth(int depth);

};
