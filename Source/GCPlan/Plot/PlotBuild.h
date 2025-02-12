#pragma once

#include "../BuildingStructsActor.h"

class PlotBuild {
public:
	PlotBuild();
	~PlotBuild();

	static TArray<FLand> CreateLands(TMap<FString, FPolygon> polygons, float unitDiameter = 9);
	static void DrawLands(TArray<FLand> lands);
	static void FlowerHomePlants(TMap<FString, FPolygon> homePlotPaths);
	static void RingPlantsCenter(TArray<FVector> vertices, float unitDiameter, int crossUnitsCount);
};
