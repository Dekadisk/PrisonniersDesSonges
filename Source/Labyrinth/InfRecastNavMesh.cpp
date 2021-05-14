// Fill out your copyright notice in the Description page of Project Settings.


#include "InfRecastNavMesh.h"
#include "Kismet/GameplayStatics.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "NavMesh/RecastQueryFilter.h"
#include "Runtime/Navmesh/Public/Detour/DetourNavMeshQuery.h"
#include "Runtime/Navmesh/Public/Detour/DetourNavMesh.h"
#include "Runtime/Navmesh/Public/Detour/DetourCommon.h"
/*
* dtQueryFilter_INF CPP
*/
float dtQueryFilter_INF::getVirtualCost(const float* pa, const float* pb,
	const dtPolyRef prevRef, const dtMeshTile* prevTile, const dtPoly* prevPoly,
	const dtPolyRef curRef, const dtMeshTile* curTile, const dtPoly* curPoly,
	const dtPolyRef nextRef, const dtMeshTile* nextTile, const dtPoly* nextPoly) const {
	const float Cost = GetCostInfluenceMap(FVector2D(-pa[0], -pa[2]), FVector2D(-pb[0], -pb[2]));
	return dtVdist(pa, pb) * Cost;
}
bool dtQueryFilter_INF::passVirtualFilter(const dtPolyRef ref, const dtMeshTile* tile, const dtPoly* poly) const {
	return passInlineFilter(ref, tile, poly);
}
float dtQueryFilter_INF::GetCostInfluenceMap(const FVector2D StartPosition, const FVector2D EndPosition) const {
	return 1.f / labGen->GetCellInfluenceAtPos({ EndPosition.X, EndPosition.Y, 0 });
}

/*
* FRecatsQueryFilter_INF CPP
*/
FRecatsQueryFilter_INF::FRecatsQueryFilter_INF(bool bIsVirtual) : dtQueryFilter_INF(bIsVirtual) {
	SetExcludedArea(RECAST_NULL_AREA);
}
INavigationQueryFilterInterface* FRecatsQueryFilter_INF::CreateCopy() const {
	return new FRecatsQueryFilter_INF(*this);
}
void FRecatsQueryFilter_INF::SetIsVirtual(bool bIsVirtual)
{
	dtQueryFilter* Filter = static_cast<dtQueryFilter*>(this);
	Filter = new(Filter)dtQueryFilter(bIsVirtual);
	SetExcludedArea(RECAST_NULL_AREA);
}
void FRecatsQueryFilter_INF::Reset()
{
	dtQueryFilter* Filter = static_cast<dtQueryFilter*>(this);
	Filter = new(Filter) dtQueryFilter(isVirtual);
	SetExcludedArea(RECAST_NULL_AREA);
}
void FRecatsQueryFilter_INF::SetAreaCost(uint8 AreaType, float Cost)
{
	setAreaCost(AreaType, Cost);
}
void FRecatsQueryFilter_INF::SetFixedAreaEnteringCost(uint8 AreaType, float Cost)
{
#if WITH_FIXED_AREA_ENTERING_COST
	setAreaFixedCost(AreaType, Cost);
#endif // WITH_FIXED_AREA_ENTERING_COST
}
void FRecatsQueryFilter_INF::SetExcludedArea(uint8 AreaType)
{
	setAreaCost(AreaType, DT_UNWALKABLE_POLY_COST);
}
void FRecatsQueryFilter_INF::SetAllAreaCosts(const float* CostArray, const int32 Count)
{
	// @todo could get away with memcopying to m_areaCost, but it's private and would require a little hack
	// need to consider if it's wort a try (not sure there'll be any perf gain)
	if (Count > RECAST_MAX_AREAS)
	{
		UE_LOG(LogNavigation, Warning, TEXT("FRecastQueryFilter_Example: Trying to set cost to more areas than allowed! Discarding redundant values."));
	}

	const int32 ElementsCount = FPlatformMath::Min(Count, RECAST_MAX_AREAS);
	for (int32 i = 0; i < ElementsCount; ++i)
	{
		setAreaCost(i, CostArray[i]);
	}
}
void FRecatsQueryFilter_INF::GetAllAreaCosts(float* CostArray, float* FixedCostArray, const int32 Count) const
{
	const float* DetourCosts = getAllAreaCosts();
	const float* DetourFixedCosts = getAllFixedAreaCosts();

	FMemory::Memcpy(CostArray, DetourCosts, sizeof(float) * FMath::Min(Count, RECAST_MAX_AREAS));
	FMemory::Memcpy(FixedCostArray, DetourFixedCosts, sizeof(float) * FMath::Min(Count, RECAST_MAX_AREAS));
}
void FRecatsQueryFilter_INF::SetBacktrackingEnabled(const bool bBacktracking)
{
	setIsBacktracking(bBacktracking);
}
bool FRecatsQueryFilter_INF::IsBacktrackingEnabled() const
{
	return getIsBacktracking();
}
float FRecatsQueryFilter_INF::GetHeuristicScale() const { return getHeuristicScale(); }
bool FRecatsQueryFilter_INF::IsEqual(const INavigationQueryFilterInterface* Other) const
{
	// @NOTE: not type safe, should be changed when another filter type is introduced
	return FMemory::Memcmp(this, Other, sizeof(FRecastQueryFilter)) == 0;
}
void FRecatsQueryFilter_INF::SetIncludeFlags(uint16 Flags)
{

	setIncludeFlags(Flags);

}
uint16 FRecatsQueryFilter_INF::GetIncludeFlags() const
{
	return getIncludeFlags();
}
void FRecatsQueryFilter_INF::SetExcludeFlags(uint16 Flags)
{
	setExcludeFlags(Flags);
}
uint16 FRecatsQueryFilter_INF::GetExcludeFlags() const
{
	return getExcludeFlags();
}


/*
* AInfRecastNavMesh CPP
*/
AInfRecastNavMesh::AInfRecastNavMesh(const FObjectInitializer& ObjectInitializer)
{
	PrimaryActorTick.bCanEverTick = true;
	DefaultNavFilter = new FRecatsQueryFilter_INF{};
}
void AInfRecastNavMesh::BeginPlay() {
	Super::BeginPlay();
	SetupCustomNavFilter();
}
void AInfRecastNavMesh::Tick(float deltaTime)
{
	Super::Tick(deltaTime);
}
void AInfRecastNavMesh::SetupCustomNavFilter() {
	if (DefaultQueryFilter.IsValid())
	{
		DefaultNavFilter->labGen = Cast<ALabGenerator>(UGameplayStatics::GetActorOfClass(GetWorld(), ALabGenerator::StaticClass()));
		DefaultQueryFilter->SetFilterImplementation(dynamic_cast<const INavigationQueryFilterInterface*>(DefaultNavFilter));
	}
}