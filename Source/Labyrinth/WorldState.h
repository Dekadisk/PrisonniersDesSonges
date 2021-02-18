#pragma once

#include <map>
#include "CoreMinimal.h"

class WorldState
{
private:

	std::map<FString, bool> states;

public:

	WorldState() = default;

	~WorldState() = default;

	WorldState(const std::map<FString, bool>& _states) : states{ _states } {}

	bool operator==(WorldState w);

	bool isIncluded(WorldState w);

	const std::map<FString, bool>& getStates();

	void setStates(const std::map<FString, bool>& states);

	void addState(FString name, bool value);

	void cleanStates();

	// Mixes two WorldState.
	void joinWorldState(WorldState w);

	bool isEmpty();
};
