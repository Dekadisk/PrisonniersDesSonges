#pragma once

#include "GOAPAction.h"
#include "CoreMinimal.h"

class GOAPNode
{
private:

	WorldState world;

	int h;

	float g;

	int parent;

	// Chosen action to reach this node.
	UGOAPAction* action;

public:
	GOAPNode() : action{ NULL }, g{ 0.0f }, h{ 0 } {}

	GOAPNode(UGOAPAction* action);

	//OPERATORS 
	bool operator==(GOAPNode node);

	// GETS

	int getH();

	float getG();

	float getF();

	int getParent();

	WorldState getWorld();

	UGOAPAction* getAction();

	// SETS

	void setH(int value);

	void setH(WorldState ws);

	void setG(GOAPNode node);

	void setParent(int p);

	void setWorld(WorldState ws);

	void setAction(UGOAPAction* action);
};
