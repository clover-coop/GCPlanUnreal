#pragma once

#include "../../BuildingStructsActor.h"
#include "../../Modeling/ModelBase.h"

class LMConferenceRoom {
public:
	LMConferenceRoom();
	~LMConferenceRoom();

	static TMap<FString, FPolygon> Create(FVector size, FModelParams modelParams = FModelParams(),
		FModelCreateParams createParamsIn = FModelCreateParams());
};
