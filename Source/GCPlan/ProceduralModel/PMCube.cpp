#include "PMCube.h"

#include "Engine/StaticMeshActor.h"
#include "ProceduralMeshComponent.h"

#include "../Common/Lodash.h"
#include "../Common/UnrealGlobal.h"
// #include "../Mesh/DynamicMaterial.h"
// #include "../Mesh/LoadContent.h"
#include "../Modeling/ModelBase.h"
#include "PMCorner.h"
#include "PMPlane.h"
#include "PMBase.h"

PMCube::PMCube() {
}

PMCube::~PMCube() {
}

void PMCube::CreateFromInputs() {
	PMBase* pmBase = PMBase::GetInstance();
	auto [inputs, modelParams] = pmBase->GetInputs("Cube", FVector(4,5,3));
	// FString name = Lodash::GetInstanceId(inputs.name);
	// if (inputs.tags.Contains("roundedCorners")) {
	// 	RoundedCorner(inputs.name + "RoundedCorner", inputs.size, inputs.tags, modelParams);
	if (inputs.tags.Contains("roundedTop")) {
		RoundedTop(inputs.name + "_RoundedTop", inputs.size, inputs.tags, 1, modelParams);
	} else if (inputs.tags.Contains("plane")) {
		Plane(inputs.name + "_Plane", inputs.size, inputs.tags, modelParams);
	} else {
		Create(inputs.name, inputs.size, inputs.tags, modelParams);
	}
}

void PMCube::Create(FString name, FVector size, TArray<FString> tags, FModelParams modelParams) {
	// UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();

	ModelBase* modelBase = ModelBase::GetInstance();
	PMBase* pmBase = PMBase::GetInstance();

	// FVector sizePerVertex = FVector(size.X / (vertices.X - 1), size.Y / (vertices.Y - 1),
	// 	size.Z / (vertices.Z - 1));

	// Parent container
	AStaticMeshActor* actor = modelBase->CreateActor(name);
	USceneComponent* parent = actor->FindComponentByClass<USceneComponent>();
	// UObject* parentObject = (UObject*)actor;
	// UProceduralMeshComponent* proceduralMesh = PMBase::CreateMesh("Cube1", parentObject);

	// float x;
	// TArray<FVector> Vertices = {};
	// TArray<FVector2D> UV0 = {};
	// TArray<int> Triangles = {};
	// int vertexIndex, vertexBottomLeftIndex, vertexBottomRightIndex,
	// 	vertexTopLeftIndex, vertexTopRightIndex;

	// // Cube has 6 sides.
	// UE_LOG(LogTemp, Display, TEXT("vertices %s"), *vertices.ToString());
	// // Front: YZ (X is constant)
	// x = size.X * -0.5;
	// for (int zz = 0; zz < vertices.Z; zz++) {
	// 	for (int yy = 0; yy < vertices.Y; yy++) {
	// 		Vertices.Add(FVector(x, yy * sizePerVertex.Y * unrealGlobal->GetScale(), zz * sizePerVertex.Z * unrealGlobal->GetScale()));
	// 		UV0.Add(FVector2D((float)yy * createParams.UVScale.X, (float)zz * createParams.UVScale.Y));

	// 		// Do 1 quad (6 triangles, 2 vertices) at a time so go every other time.
	// 		if (yy % 2 == 1 && zz < (vertices.Z - 1)) {
	// 			vertexIndex = (zz * vertices.Y) + yy;
	// 			vertexBottomRightIndex = vertexIndex;
	// 			vertexBottomLeftIndex = vertexIndex - 1;
	// 			vertexTopRightIndex = ((zz + 1) * vertices.Y) + yy;
	// 			vertexTopLeftIndex = vertexTopRightIndex - 1;
	// 			Triangles.Add(vertexBottomLeftIndex);
	// 			Triangles.Add(vertexBottomRightIndex);
	// 			Triangles.Add(vertexTopLeftIndex);
	// 			Triangles.Add(vertexBottomRightIndex);
	// 			Triangles.Add(vertexTopRightIndex);
	// 			Triangles.Add(vertexTopLeftIndex);
	// 			UE_LOG(LogTemp, Display, TEXT("Triangles %d %d %d %d %d %d"), vertexBottomLeftIndex, vertexBottomRightIndex, vertexTopLeftIndex, vertexBottomRightIndex, vertexTopRightIndex, vertexTopLeftIndex);
	// 		}
	// 	}
	// }

	// // TODO - other sides

	// PMBase::AddMeshSection(proceduralMesh, Vertices, UV0, Triangles);

	TArray<FPlaneOffsets> xOffsets = {
		{ FPlaneOffsets(0, 0, 0, 0) },
		{ FPlaneOffsets(1, 0, 0, 0) }
	};
	TArray<FPlaneOffsets> yOffsets = {
		{ FPlaneOffsets(0, 0, 0, 0) },
		{ FPlaneOffsets(1, 0, 0, 0) }
	};
	TArray<FString> tagsPlane = {};

	FModelCreateParams createParams;
	createParams.parentActor = actor;
	createParams.parent = parent;
	// // This works too
	// FVector sizePlane = FVector(size.X, size.Y, 0.01);
	// createParams.offset = FVector(0,0,0);
	// createParams.triangleSide = -1;
	// PMPlane::Create(name + "_Bottom", sizePlane, xOffsets, yOffsets, "xy", tagsPlane, createParams);
	// createParams.offset = FVector(0, -1 * size.Y / 2, 0);
	// createParams.triangleSide = 1;
	// PMPlane::Create(name + "_Left", sizePlane, xOffsets, yOffsets, "xz", tagsPlane, createParams);
	// createParams.offset = FVector(-1 * size.X / 2, 0, 0);
	// PMPlane::Create(name + "_Back", sizePlane, xOffsets, yOffsets, "yz", tagsPlane, createParams);
	// createParams.offset = FVector(0,0,size.Z);
	// PMPlane::Create(name + "_Top", sizePlane, xOffsets, yOffsets, "xy", tagsPlane, createParams);
	// createParams.offset = FVector(0, size.Y / 2, 0);
	// createParams.triangleSide = -1;
	// PMPlane::Create(name + "_Right", sizePlane, xOffsets, yOffsets, "xz", tagsPlane, createParams);
	// createParams.offset = FVector(size.X / 2, 0, 0);
	// PMPlane::Create(name + "_Front", sizePlane, xOffsets, yOffsets, "yz", tagsPlane, createParams);

	FVector sizeXZ = PMPlane::MapXYZ(size, "xz");
	FVector sizeYZ = PMPlane::MapXYZ(size, "yz");
	createParams.offset = FVector(0,0,0);
	// This works too (use full size, which makes offsets simpler).
	createParams.triangleSide = -1;
	PMPlane::Create(name + "_Bottom", size, xOffsets, yOffsets, "xy", tagsPlane, createParams);
	createParams.triangleSide = 1;
	PMPlane::Create(name + "_Left", sizeXZ, xOffsets, yOffsets, "xz", tagsPlane, createParams);
	PMPlane::Create(name + "_Back", sizeYZ, xOffsets, yOffsets, "yz", tagsPlane, createParams);
	createParams.offset = FVector(0,0,size.Z);
	PMPlane::Create(name + "_Top", size, xOffsets, yOffsets, "xy", tagsPlane, createParams);
	createParams.offset = FVector(0,size.Y,0);
	createParams.triangleSide = -1;
	PMPlane::Create(name + "_Right", sizeXZ, xOffsets, yOffsets, "xz", tagsPlane, createParams);
	createParams.offset = FVector(size.X,0,0);
	PMPlane::Create(name + "_Front", sizeYZ, xOffsets, yOffsets, "yz", tagsPlane, createParams);
}

// This creates a 5 sided (no bottom) cube by making a pyramid that is nearly vertically straight up.
// It works, but the UVs will likely be messed up, so do not use?
AActor* PMCube::Plane(FString name, FVector size, TArray<FString> tags,
	FModelParams modelParams) {
	ModelBase* modelBase = ModelBase::GetInstance();
	AActor* actor = modelBase->CreateActor(name);
	USceneComponent* parent = actor->FindComponentByClass<USceneComponent>();
	FModelCreateParams createParams;
	createParams.parentActor = actor;
	createParams.parent = parent;

	// LoadContent* loadContent = LoadContent::GetInstance();
	// DynamicMaterial* dynamicMaterial = DynamicMaterial::GetInstance();
	// FString texturePathBase = loadContent->Texture("leather_base");
	// FString texturePathNormal = loadContent->Texture("leather_normal");
	// modelParams.dynamicMaterial = dynamicMaterial->CreateTextureColor(name + "_leather", texturePathBase,
	// 	texturePathNormal, DynamicMaterial::GetColor("beige"));

	TArray<FPlaneOffsets> offsets = {
		{ FPlaneOffsets(0, 0, 0, 0) },
		{ FPlaneOffsets(0.01, 0, 0, 1) },
		{ FPlaneOffsets(0.99, 0, 0, 1) },
		{ FPlaneOffsets(1, 0, 0, 0) }
	};
	createParams.offsetsCombine = { { "z", "min" } };
	PMPlane::Create(name + "_Cube", size, offsets, offsets, "xy", tags, createParams, modelParams);
	return actor;
}

AStaticMeshActor* PMCube::RoundedTop(FString name, FVector size, TArray<FString> tags,
	float topHeight, FModelParams modelParams) {
	FVector parentLocation = modelParams.location;
	modelParams.location = FVector(0,0,0);
	ModelBase* modelBase = ModelBase::GetInstance();
	AStaticMeshActor* actor = modelBase->CreateActor(name);
	modelParams.location = FVector(0,0,0);
	USceneComponent* parent = actor->FindComponentByClass<USceneComponent>();
	TArray<FPlaneOffsets> offsetsFlat = {
		{ FPlaneOffsets(0, 0, 0, 0) },
		{ FPlaneOffsets(1, 0, 0, 0) }
	};
	FVector sizeSides = FVector(size.X, size.Y, size.Z - topHeight);
	FVector sizeTop = FVector(size.X, size.Y, topHeight);
	FVector sizeSidesXZ = PMPlane::MapXYZ(sizeSides, "xz");
	FVector sizeSidesYZ = PMPlane::MapXYZ(sizeSides, "yz");
	FModelCreateParams createParams;
	createParams.parentActor = actor;
	createParams.parent = parent;

	// UObject* parentObject = (UObject*)createParams.parentActor;
	// createParams.proceduralMesh = PMBase::CreateMesh(name + "_RoundedTop", parentObject);

	createParams.triangleSide = -1;
	PMPlane::Create(name + "_Bottom", size, offsetsFlat, offsetsFlat, "xy", tags, createParams, modelParams);
	createParams.triangleSide = 1;
	PMPlane::Create(name + "_Left", sizeSidesXZ, offsetsFlat, offsetsFlat, "xz", tags, createParams, modelParams);
	PMPlane::Create(name + "_Back", sizeSidesYZ, offsetsFlat, offsetsFlat, "yz", tags, createParams, modelParams);
	createParams.offset = FVector(0,size.Y,0);
	createParams.triangleSide = -1;
	PMPlane::Create(name + "_Right", sizeSidesXZ, offsetsFlat, offsetsFlat, "xz", tags, createParams, modelParams);
	createParams.offset = FVector(size.X,0,0);
	PMPlane::Create(name + "_Front", sizeSidesYZ, offsetsFlat, offsetsFlat, "yz", tags, createParams, modelParams);

	// TArray<FPlaneOffsets> offsetsTop = PMPlane::BorderRadiusTop(0.5, 0.25);
	// offsetsTop += PMPlane::BorderRadiusTop(0.5, 0.25, "end");
	// createParams.triangleSide = 1;
	// PMPlane::Create(name + "_Top", sizeTop, offsetsTop, offsetsTop, "xy", tags, createParams, {{ "z", "min" }});
	// createParams.offset = FVector(0,size.Y,0);

	createParams.offset = FVector(0,0,size.Z - topHeight);
	RoundedTopSide(sizeTop, {}, createParams, modelParams);

	ModelBase::SetTransform(actor, parentLocation);
	return actor;
}

void PMCube::RoundedTopSide(FVector size, TArray<FString> tags, FModelCreateParams createParams,
	FModelParams modelParams) {
	FVector offsetParent = createParams.offset;
	ModelBase* modelBase = ModelBase::GetInstance();
	AActor* actorBase;
	FVector location = FVector(0,0,0);
	FVector rotation = FVector(0,0,0);
	FVector scale = FVector(1,1,1);
	FActorSpawnParameters spawnParams;
	// FModelParams modelParams;
	modelParams.parent = createParams.parent;
	FString name;
	UProceduralMeshComponent* proceduralMesh;

	// Use different parent for cloner.
	AStaticMeshActor* clonerActor = modelBase->CreateActor(Lodash::GetInstanceId("Cloner_"), -1 * createParams.offset);
	USceneComponent* clonerParent = clonerActor->FindComponentByClass<USceneComponent>();
	FModelParams clonerModelParams = modelParams;
	clonerModelParams.parent = clonerParent;
	FModelCreateParams clonerCreateParams;
	clonerCreateParams.parentActor = clonerActor;
	clonerCreateParams.parent = clonerParent;
	createParams.offset = FVector(0,0,0);

	// 4 corners
	FVector sizeCorner = FVector(size.Z, size.Z, size.Z);
	// Create instance with ZERO rotation or offset, since will be cloning it.
	clonerCreateParams.UVScale = FVector2D(0.01, 0.01);
	actorBase = PMCorner::Create(sizeCorner, tags, clonerCreateParams, clonerModelParams);
	// actorBase = PMBase::MeshToActor(Lodash::GetInstanceId("Corner_"), proceduralMesh, clonerCreateParams, clonerModelParams);
	spawnParams.Template = actorBase;
	name = actorBase->GetName();
	float xBack = -1 * size.X / 2 + sizeCorner.X / 2;
	float yLeft = -1 * size.Y / 2 + sizeCorner.Y / 2;
	float xFront = size.X / 2 - sizeCorner.X / 2;
	float yRight = size.Y / 2 - sizeCorner.Y / 2;

	location = FVector(xBack, yLeft, 0);
	modelParams.rotation = FVector(0,0,0);
	modelBase->CreateActor(name + "_BackLeft", location, rotation, scale, spawnParams, modelParams);

	location = FVector(xFront, yLeft, 0);
	modelParams.rotation = FVector(0,0,90);
	modelBase->CreateActor(name + "_FrontLeft", location, rotation, scale, spawnParams, modelParams);

	location = FVector(xFront, yRight, 0);
	modelParams.rotation = FVector(0,0,180);
	modelBase->CreateActor(name + "_FrontRight", location, rotation, scale, spawnParams, modelParams);

	location = FVector(xBack, yRight, 0);
	modelParams.rotation = FVector(0,0,270);
	modelBase->CreateActor(name + "_BackRight", location, rotation, scale, spawnParams, modelParams);

	actorBase->Destroy();

	// Reset.
	modelParams.rotation = FVector(0,0,0);
	// 4 edges
	FVector sizeEdge = FVector(sizeCorner.X, size.Y - sizeCorner.Y * 2, sizeCorner.Z);
	TArray<FString> tagsEdge = tags;
	tagsEdge += { "oneSide" };
	clonerCreateParams.UVScale = FVector2D(0.01, 1);
	actorBase = PMCorner::Create(sizeEdge, tagsEdge, clonerCreateParams, clonerModelParams);
	// actorBase = PMBase::MeshToActor(Lodash::GetInstanceId("Edge_"), proceduralMesh, clonerCreateParams, clonerModelParams);
	spawnParams.Template = actorBase;
	name = actorBase->GetName();

	scale.Y = 1;
	location = FVector(-1 * size.X / 2 + sizeCorner.X / 2, 0, 0);
	modelParams.rotation = FVector(0,0,0);
	modelBase->CreateActor(name + "_Back", location, rotation, scale, spawnParams, modelParams);	

	scale.Y = (size.X - sizeCorner.X * 2) / sizeEdge.Y;
	location = FVector(0, -1 * size.Y / 2 + sizeCorner.Y / 2, 0);
	modelParams.rotation = FVector(0,0,90);
	modelBase->CreateActor(name + "_Left", location, rotation, scale, spawnParams, modelParams);

	scale.Y = 1;
	location = FVector(size.X / 2 - sizeCorner.X / 2, 0, 0);
	modelParams.rotation = FVector(0,0,180);
	modelBase->CreateActor(name + "_Front", location, rotation, scale, spawnParams, modelParams);

	scale.Y = (size.X - sizeCorner.X * 2) / sizeEdge.Y;
	location = FVector(0, size.Y / 2 - sizeCorner.Y / 2, 0);
	modelParams.rotation = FVector(0,0,270);
	modelBase->CreateActor(name + "_Right", location, rotation, scale, spawnParams, modelParams);

	scale.Y = 1;
	actorBase->Destroy();
	clonerActor->Destroy();

	// Middle
	modelParams.rotation = FVector(0,0,0);
	createParams.offset = offsetParent + FVector(0,0,size.Z);
	FVector sizeTop = FVector(size.X - sizeCorner.X * 2, size.Y - sizeCorner.Y * 2, 0.01);
	createParams.triangleSide = 1;
	PMPlane::Create(name + "_Top", sizeTop, {}, {}, "xy", {}, createParams, modelParams);
}
