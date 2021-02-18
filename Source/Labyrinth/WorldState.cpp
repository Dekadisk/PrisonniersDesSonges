#include "WorldState.h"

bool WorldState::operator==(WorldState  w)
{
	return states.size() == w.getStates().size() && std::equal(states.begin(), states.end(), w.getStates().begin());
}

bool WorldState::isIncluded(WorldState w)
{
	for (auto requirement : w.getStates())
	{
		auto it = states.find(requirement.first);
		if (it != states.end())
		{
			if (it->second != requirement.second)
				return false;
		}
		else return false;
	}
	return true;

}

const std::map<FString, bool>& WorldState::getStates()
{
	return states;
}

void WorldState::setStates(const std::map<FString, bool>& a)
{
	states = a;
}

void WorldState::addState(FString name, bool value)
{
	states[name] = value;
}

void WorldState::cleanStates()
{
	states.clear();
}

void WorldState::joinWorldState(WorldState w)
{
	for (auto state : w.getStates())
	{
		states[state.first] = state.second;
	}
}

bool WorldState::isEmpty()
{
	return states.size() == 0;
}
