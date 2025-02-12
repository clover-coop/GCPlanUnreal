#include "ProceduralModelActor.h"

#include "Common/UnrealGlobal.h"
#include "Modeling/ModelBase.h"
#include "ProceduralModel/PMBase.h"

AProceduralModelActor::AProceduralModelActor()
{
	PrimaryActorTick.bCanEverTick = false;
}

void AProceduralModelActor::Init() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	unrealGlobal->InitAll(GetWorld(), { "web" });
	// ModelBase* modelBase = ModelBase::GetInstance();
	// modelBase->SetWorld(GetWorld());

	// PMBase* pmBase = PMBase::GetInstance();
	// pmBase->SetWorld(GetWorld());
}

void AProceduralModelActor::GenerateModel() {
	Init();
	PMBase* pmBase = PMBase::GetInstance();
	pmBase->SetInputs(ProceduralModelBase);
	pmBase->Create();
}

void AProceduralModelActor::DestroyActors() {
	Init();
	ModelBase* modelBase = ModelBase::GetInstance();
	modelBase->DestroyActors();
}
