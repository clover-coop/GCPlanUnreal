#include "LMLobby.h"

#include "Engine/StaticMeshActor.h"

#include "LayoutModelBase.h"
#include "../Modeling/ModelBase.h"
#include "../Mesh/LoadContent.h"
#include "../ModelingStructsActor.h"

LMLobby::LMLobby() {
}

LMLobby::~LMLobby() {
}

void LMLobby::Create() {
	ModelBase* modelBase = ModelBase::GetInstance();
	LayoutModelBase* layoutModelBase = LayoutModelBase::GetInstance();
	UWorld* World = layoutModelBase->GetWorld();
	FLayoutModelBaseParams layoutParams = layoutModelBase->GetInputs("Lobby1", FVector(25,25,4));
	FString name = layoutParams.name;
	FVector size = layoutParams.size;
	TArray<FString> tags = layoutParams.tags;

	FRotator rotation = FRotator(0,0,0);
	FActorSpawnParameters spawnParams;
	FVector location = FVector(0,0,0);
	FVector scale = FVector(1,1,1);
	AStaticMeshActor* actor;

	// Parent container
	actor = modelBase->CreateActor(name, location, rotation, scale, spawnParams);
	USceneComponent* parent = actor->FindComponentByClass<USceneComponent>();

	LoadContent* loadContent = LoadContent::GetInstance();
	FString meshPath = "/Script/Engine.StaticMesh'/Game/Modeling/Primitives/Cube.Cube'";
	FString materialPath = loadContent->Material("white");

	// Floor
	// TODO

	// Couch
	location = FVector(0, 2, 0);
	scale = FVector(1,1,1);
	modelBase->CreateActor(name + "_Couch", location, rotation, scale, spawnParams, parent, meshPath, materialPath);

	// Ceiling
	// TODO

	// TODO
}
