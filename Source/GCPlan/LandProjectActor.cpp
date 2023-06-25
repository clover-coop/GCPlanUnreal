#include "LandProjectActor.h"
#include "JsonObjectConverter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"

#include "BuildingStructsActor.h"
#include "DataStructsActor.h"
#include "SocketActor.h"

#include "Common/UnrealGlobal.h"
#include "Draw/DrawVertices.h"
#include "Landscape/SplineRoad.h"
#include "Landscape/VerticesEdit.h"
#include "Mesh/InstancedMesh.h"
#include "Plot/PlotBuild.h"

#include "LayoutModel/OfficeRoom/LMCafeteria.h"
#include "LayoutModel/OfficeRoom/LMConferenceRoom.h"
#include "LayoutModel/OfficeRoom/LMLobby.h"
#include "LayoutModel/OfficeRoom/LMOfficeDesks.h"
#include "LayoutModel/OfficeRoom/LMServerRoom.h"

#include "Common/MathVector.h"
#include "Layout/LayoutPolygon.h"

ALandProjectActor::ALandProjectActor()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ALandProjectActor::BeginPlay()
{
	Super::BeginPlay();
}

void ALandProjectActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!Inited) {
		Init();
	}
}

void ALandProjectActor::InitSocketOn() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();

	unrealGlobal->SocketActor->On("login", [this, unrealGlobal](FString DataString) {
		FDataLogin* Data = new FDataLogin();
		if (!FJsonObjectConverter::JsonObjectStringToUStruct(DataString, Data, 0, 0)) {
			UE_LOG(LogTemp, Error, TEXT("LandProjectActor.On login json parse error"));
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "LandProjectActor.On login json parse error");
		} else {
			if (Data->valid > 0) {
				FString ProjectUName = unrealGlobal->_settings->projectUName;
				if (ProjectUName.Len() > 0) {
					this->GetProject(ProjectUName);
				}
			}
		}
	});

	unrealGlobal->SocketActor->On("projectGetByUName", [this](FString DataString) {
		FDataProject* Data = new FDataProject();
		if (!FJsonObjectConverter::JsonObjectStringToUStruct(DataString, Data, 0, 0)) {
			UE_LOG(LogTemp, Error, TEXT("LandProjectActor.On projectGetByUName json parse error"));
			// GEngine->AddOnScreenDebugMessage(-1, 15.0f, FColor::Red, "LandProjectActor.On projectGetByUName json parse error");
		} else {
			if (Data->valid > 0) {
				PlotBuild::DrawLands(Data->lands);
			}
		}
	});
}

void ALandProjectActor::Login() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	FString LoginEmail = unrealGlobal->_settings->loginEmail;
	FString LoginPassword = unrealGlobal->_settings->loginPassword;
	FString ProjectUName = unrealGlobal->_settings->projectUName;
	if (LoginEmail.Len() > 0 && LoginPassword.Len() > 0 && ProjectUName.Len() > 0) {
		TMap<FString, FString> Data = {
			{ "email", LoginEmail },
			{ "password", LoginPassword }
		};
		unrealGlobal->SocketActor->Emit("login", Data);
	}
}

void ALandProjectActor::Init() {
	TArray<FString> Keys = {"socket"};
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	// unrealGlobal->InitAll(GetWorld());
	if (unrealGlobal->IsIniteds(Keys) && unrealGlobal->SocketActor->IsConnected()) {
		Inited = true;
		this->InitSocketOn();
		this->Login();

		VerticesEdit* verticesEdit = VerticesEdit::GetInstance();
		verticesEdit->LoadFromFiles();
		verticesEdit->CheckSubdividePolygons("plot");
	}
}

void ALandProjectActor::GetProject(FString UName) {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	TMap<FString, FString> Data = {
		{ "uName", UName },
		{ "getPlotsSupercells", "1" }
	};
	unrealGlobal->SocketActor->Emit("projectGetByUName", Data);
}

void ALandProjectActor::TakeAction() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	unrealGlobal->InitAll(GetWorld());

	if (EditorParams.Action == EditorActionsLP::PLOTSREMOVECHILDREN) {
		VerticesEdit* verticesEdit = VerticesEdit::GetInstance();
		verticesEdit->LoadFromFiles();
		verticesEdit->RemoveChildren("plot");
		UE_LOG(LogTemp, Display, TEXT("Plots children cleared"));
	}
}

void ALandProjectActor::Test() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	unrealGlobal->InitAll(GetWorld());

	// LayoutPolygon* layoutPolygon = LayoutPolygon::GetInstance();
	// TArray<FVector> vertices = {
	// 	// FVector(0,0,0), FVector(0,0,4), FVector(0,3,4), FVector(0,3,0)
	// 	// FVector(-1,1.5,0), FVector(-1,1.5,4), FVector(1.5,-2,4), FVector(1.5,-2,0)
	// 	FVector(-1,-2,0), FVector(-1,-2,4), FVector(1.5,1.5,4), FVector(1.5,1.5,0)
	// };
	// TArray<FString> meshNames = { "brackenFern", "solidFern", "cinnamonFern" };
	// FPlaceParams placeParams;
	// placeParams.plane = "yz";
	// placeParams.offsetAverage = 0.3;
	// placeParams.rotMinX = 90;
	// placeParams.rotMaxX = 90;
	// placeParams.rotMinZ = 270;
	// placeParams.rotMaxZ = 270;
	// placeParams.rotMinY = 0;
	// placeParams.rotMaxY = 360;
	// LayoutPolygon::PlaceInPolygon(vertices, meshNames, placeParams);


	// FModelParams modelParams;
	// FModelCreateParams createParams;
	// LMOfficeDesks::Desks(FVector(10,12,4), modelParams, createParams);

	TArray<FVector> vertices = { FVector(1, -1, 0) };
	FVector rotation = FVector(0,0,180);
	// rotation = FVector(0,0,30);
	vertices = MathVector::RotateAround(vertices, rotation, FVector(0,0,0));
	UE_LOG(LogTemp, Display, TEXT("rotate around %s"), *vertices[0].ToString());
}

void ALandProjectActor::Clear() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	unrealGlobal->InitAll(GetWorld());

	SplineRoad* splineRoad = SplineRoad::GetInstance();
	splineRoad->CleanUp();

	// Must be last (after any others that call instanced mesh).
	InstancedMesh* instancedMesh = InstancedMesh::GetInstance();
	instancedMesh->CleanUp();

	// Not fixing the issue..
	// GEngine->ForceGarbageCollection(true);
}

void ALandProjectActor::DrawVertices() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	unrealGlobal->InitAll(GetWorld());

	// this->InitSocketOn();
	// this->Login();

	DrawVertices::LoadVertices();
}

void ALandProjectActor::SetVertices() {
	UnrealGlobal* unrealGlobal = UnrealGlobal::GetInstance();
	unrealGlobal->InitAll(GetWorld());

	VerticesEdit* verticesEdit = VerticesEdit::GetInstance();
	verticesEdit->DestroyItems();
	// Hardcoded - varies by project (usually these are manually drawn / set via UI).
	FModelParams modelParams;
	FModelCreateParams createParams;
	createParams.rotation = FVector(0,0,30);
	createParams.offset = FVector(-285,20,3);
	LMLobby::Create(FVector(10,12,4), modelParams, createParams);
	createParams.offset = FVector(-279,9,3);
	LMOfficeDesks::Desks(FVector(11,12,4), modelParams, createParams);
	createParams.offset = FVector(-275,25,3);
	LMConferenceRoom::Create(FVector(10,12,4), modelParams, createParams);
	createParams.offset = FVector(-269,15,3);
	LMLobby::TwoTables(FVector(10,12,4), modelParams, createParams);
	createParams.offset = FVector(-291,31,3);
	LMCafeteria::Create(FVector(10,12,4), modelParams, createParams);
	createParams.offset = FVector(0,0,4);
	LMServerRoom::Create(FVector(10,12,4), modelParams, createParams);
}
