#include "GOAPAction.h"

void UGOAPAction::create_P_E()
{
	for (FState itP : preconditions)
	{
		wsPreconditions.addState(itP.name, itP.value);
	}
	for (FState itE : effects)
	{
		wsEffects.addState(itE.name, itE.value);
	}
	if (targetsType == NULL)
		UE_LOG(LogTemp, Warning, TEXT("Targets' type of '%s' action are not defined."), *name);
}

TArray<AActor*> UGOAPAction::getTargetsList(APawn* owner)
{
	TArray<AActor*> actorsFound;
	UGameplayStatics::GetAllActorsOfClass(owner->GetWorld(), targetsType, actorsFound);
	return actorsFound;
}

bool UGOAPAction::checkProceduralPrecondition(APawn* owner) {
	return true;
}

bool UGOAPAction::doAction(APawn* owner) {
	return true;
}

bool UGOAPAction::operator==(UGOAPAction& action)
{
	return cost == action.getCost() && target == action.getTarget() && wsPreconditions == action.getPreconditions() && wsEffects == action.getEffects();
}

bool UGOAPAction::operator!=(UGOAPAction& action)
{
	return !(*this == action);
}

// GETS

FString UGOAPAction::getName()
{
	return name;
}

float UGOAPAction::getCost()
{
	return cost;
}

AActor* UGOAPAction::getTarget()
{
	return target;
}

WorldState UGOAPAction::getPreconditions()
{
	return wsPreconditions;
}

WorldState UGOAPAction::getEffects()
{
	return wsEffects;
}

// SETS

void UGOAPAction::setName(FString n)
{
	name = n;
}

void UGOAPAction::setCost(float c)
{
	cost = c;
}

void UGOAPAction::setTarget(AActor* t)
{
	target = t;
}

void UGOAPAction::setPreconditions(WorldState pre)
{
	wsPreconditions = pre;
}

void UGOAPAction::setEffects(WorldState ef)
{
	wsEffects = ef;
}
