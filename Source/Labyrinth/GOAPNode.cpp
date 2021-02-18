#include "GOAPNode.h"

GOAPNode::GOAPNode(UGOAPAction* a)
{
	action = a;
	if (a != NULL) g = a->getCost();
	else g = 0;
	h = 0;
}

bool GOAPNode::operator==(GOAPNode node)
{
	return action == node.getAction();
}


WorldState GOAPNode::getWorld()
{
	return world;
}

int GOAPNode::getH()
{
	return h;
}

float GOAPNode::getG()
{
	return g;
}

float GOAPNode::getF()
{
	return g + h;
}

int GOAPNode::getParent()
{
	return parent;
}

UGOAPAction* GOAPNode::getAction()
{
	return action;
}

void GOAPNode::setWorld(WorldState ws)
{
	world = ws;
}

void GOAPNode::setH(int value)
{
	h = value;
}

void GOAPNode::setH(WorldState ws)
{
	for (auto it : world.getStates())
	{
		auto aux = ws.getStates().find(it.first);
		if (aux != ws.getStates().end())
		{
			if (it.second != aux->second) ++h;
		}
		else ++h;
	}
}

void GOAPNode::setG(GOAPNode node)
{
	g += node.getG();
}

void GOAPNode::setParent(int p)
{
	parent = p;
}

void GOAPNode::setAction(UGOAPAction* a)
{
	h = 0;
	g = a->getCost();
	action = a;
}
