// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LabGenerator.h"
#include "Detour/DetourNavMeshQuery.h"
#include "NavMesh/RecastNavMesh.h"
#include "InfRecastNavMesh.generated.h"

/*
* Default Filter
*/
class LABYRINTH_API dtQueryFilter_INF : public dtQueryFilter {
public:
	dtQueryFilter_INF(bool inIsVirtual = true) : dtQueryFilter(inIsVirtual) {}

	virtual ~dtQueryFilter_INF() {}

	ALabGenerator* labGen;
protected:
	virtual float getVirtualCost(const float* pa, const float* pb,
		const dtPolyRef prevRef, const dtMeshTile* prevTile, const dtPoly* prevPoly,
		const dtPolyRef curRef, const dtMeshTile* curTile, const dtPoly* curPoly,
		const dtPolyRef nextRef, const dtMeshTile* nextTile, const dtPoly* nextPoly) const override;

	virtual bool passVirtualFilter(const dtPolyRef ref,
		const dtMeshTile* tile,
		const dtPoly* poly) const override;
private:
	float GetCostInfluenceMap(const FVector2D StartPosition, const FVector2D EndPosition) const;
};

class LABYRINTH_API FRecatsQueryFilter_INF : public INavigationQueryFilterInterface, public dtQueryFilter_INF {
public:
	FRecatsQueryFilter_INF(bool bIsVirtual = true);
	~FRecatsQueryFilter_INF() {}

	virtual void Reset() override;

	virtual void SetAreaCost(uint8 AreaType, float Cost) override;
	virtual void SetFixedAreaEnteringCost(uint8 AreaType, float Cost) override;
	virtual void SetExcludedArea(uint8 AreaType) override;
	virtual void SetAllAreaCosts(const float* CostArray, const int32 Count) override;
	virtual void GetAllAreaCosts(float* CostArray, float* FixedCostArray, const int32 Count) const override;
	virtual void SetBacktrackingEnabled(const bool bBacktracking) override;
	virtual bool IsBacktrackingEnabled() const override;
	virtual float GetHeuristicScale() const override;
	virtual bool IsEqual(const INavigationQueryFilterInterface* Other) const override;
	virtual void SetIncludeFlags(uint16 Flags) override;
	virtual uint16 GetIncludeFlags() const override;
	virtual void SetExcludeFlags(uint16 Flags) override;
	virtual uint16 GetExcludeFlags() const override;


	virtual INavigationQueryFilterInterface* CreateCopy() const override;

	const dtQueryFilter* GetAsDetourQueryFilter() const { return this; }

	void SetIsVirtual(bool bIsVirtual);
};

/**
 * AInfRecastNavMesh
 */
UCLASS()
class LABYRINTH_API AInfRecastNavMesh : public ARecastNavMesh
{
	GENERATED_BODY()

public:
	AInfRecastNavMesh(const FObjectInitializer& ObjectInitializer);

protected:
	virtual void Tick(float deltaTime) override;
	virtual void BeginPlay() override;

private:
	FRecatsQueryFilter_INF* DefaultNavFilter{};
	void SetupCustomNavFilter();
};