#include "ModelEVCharger.h"

#include "Engine/StaticMeshActor.h"

#include "ModelBase.h"
#include "../ModelingStructsActor.h"
#include "../ProceduralModel/PMCylinder.h"

ModelEVCharger::ModelEVCharger()
{
}

ModelEVCharger::~ModelEVCharger()
{
}

void ModelEVCharger::Create()
{
    ModelBase *modelBase = ModelBase::GetInstance();
    UWorld *World = modelBase->GetWorld();
    FModelingBase modelingBase = modelBase->GetInputs("EVCharger", FVector(1, 1, 2));
    FString name = modelingBase.name;
    FVector size = modelingBase.size;
    TArray<FString> tags = modelingBase.tags;

    FRotator rotation = FRotator(0, 0, 0);
    FActorSpawnParameters spawnParams;
    FVector location = FVector(0, 0, 0);
    FVector scale = FVector(1, 1, 1);
    AStaticMeshActor *actor;

    // Parent container
    actor = modelBase->CreateActor(name, location, rotation, scale, spawnParams);
    USceneComponent *parent = actor->FindComponentByClass<USceneComponent>();

    FString meshPathCube = "/Script/Engine.StaticMesh'/Game/Modeling/Primitives/Cube.Cube'";
    FString meshPathCylinder = "/Script/Engine.StaticMesh'/Game/Modeling/Primitives/Cylinder.Cylinder'";
    FString meshPathSphere = "/Script/Engine.StaticMesh'/Game/Modeling/Primitives/Sphere.Sphere'";

    float chargerBottomHeight = size.Z * 2 / 3;
    float chargerTopHeight = size.Z / 3;
    float chargerWidth = 0.8;
    float chargerDepth = 0.3;
    float rectVerticalWidth = chargerWidth * 0.1;
    float rectHorizontalHeight = chargerTopHeight * 0.1;
    float cableLength = 2;
    float cableRadius = 0.04;

    // Charger Bottom
    location = FVector(0, 0, 0);
    scale = FVector(chargerWidth, chargerDepth, chargerBottomHeight);
    modelBase->CreateActor(name + "_ChargerBottom", location, rotation, scale, spawnParams, parent, meshPathCube);

    // Charger Top Rectangular Frame Left
    location = FVector(-(chargerWidth - rectVerticalWidth) / 2, 0, chargerBottomHeight);
    scale = FVector(rectVerticalWidth, chargerDepth, chargerTopHeight);
    modelBase->CreateActor(name + "_ChargerTopRectLeft", location, rotation, scale, spawnParams, parent, meshPathCube);

    // Charger Top Rectangular Frame Right
    location = FVector((chargerWidth - rectVerticalWidth) / 2, 0, chargerBottomHeight);
    scale = FVector(rectVerticalWidth, chargerDepth, chargerTopHeight);
    modelBase->CreateActor(name + "_ChargerTopRectRight", location, rotation, scale, spawnParams, parent, meshPathCube);

    // Charger Top Rectangular Frame Top
    location = FVector(0, 0, chargerBottomHeight + chargerTopHeight - rectHorizontalHeight);
    scale = FVector(chargerWidth, chargerDepth, rectHorizontalHeight);
    modelBase->CreateActor(name + "_ChargerTopRectTop", location, rotation, scale, spawnParams, parent, meshPathCube);

    // Screen
    location = FVector(0, chargerDepth / 2, chargerBottomHeight * 2 / 3);
    scale = FVector(chargerWidth * 0.8, 0.02, chargerBottomHeight * 0.3);
    modelBase->CreateActor(name + "_Screen", location, rotation, scale, spawnParams, parent, meshPathCube);

    /******************** Charging Cable ********************/

    // Charging Cable Section 1
    float section1Length = cableLength * 0.05;
    float cableCurrentX = (chargerWidth + section1Length) / 2;
    float cableCurrentZ = chargerBottomHeight * 0.8;
    location = FVector(cableCurrentX, 0, cableCurrentZ);
    scale = FVector(cableRadius, cableRadius, section1Length);
    modelBase->CreateActor(name + "_ChargingCable_Section1", location, FRotator(90, 0, 0), scale, spawnParams, parent, meshPathCylinder);

    // Charging Cable Section 2
    float section2Length = cableLength * 0.3;
    cableCurrentZ = cableCurrentZ - section2Length + cableRadius / 2;
    location = FVector(cableCurrentX, 0, cableCurrentZ);
    scale = FVector(cableRadius, cableRadius, section2Length);
    modelBase->CreateActor(name + "_ChargingCable_Section2", location, FRotator(0, 0, 0), scale, spawnParams, parent, meshPathCylinder);

    // Charging Cable Section 3
    float section3Length = cableLength * 0.2;
    float stepAngleDegree = 15.0;
    float circularAngleDegree = 180.0;
    FVector startLocation = FVector(cableCurrentX, 0, cableCurrentZ);
    float unitLength = section3Length * stepAngleDegree / circularAngleDegree;

    auto [locations, rotations] = BuildCircularCableLocations(section3Length, 180.0, FVector(cableCurrentX, 0, cableCurrentZ), 15.0, unitLength);
    for (int32 i = 0; i < locations.Num() - 1; i++)
    {
        location = locations[i];
        rotation = rotations[i];
        scale = FVector(cableRadius, cableRadius, unitLength);
        modelBase->CreateActor(name + "__ChargingCable_Section3_" + FString::FromInt(i), location, rotation, scale, spawnParams, parent, meshPathCylinder);
    }

    // Charging Cable Section 4
    float section4Length = cableLength * 0.15;
    cableCurrentX = cableCurrentX + section3Length * 2 / PI;
    location = FVector(cableCurrentX, 0, cableCurrentZ);
    scale = FVector(cableRadius, cableRadius, section4Length);
    modelBase->CreateActor(name + "_ChargingCable_Section4", location, FRotator(15, 0, 0), scale, spawnParams, parent, meshPathCylinder);

    // Charging Cable Section 5
    float section5Length = cableLength * 0.3;
    cableCurrentZ = cableCurrentZ + section4Length * FMath::Cos(FMath::DegreesToRadians(15.0));
    cableCurrentX = cableCurrentX - section4Length * FMath::Sin(FMath::DegreesToRadians(15.0));
    location = FVector(cableCurrentX, 0, cableCurrentZ);
    scale = FVector(cableRadius, cableRadius, section5Length);
    modelBase->CreateActor(name + "_ChargingCable_Section5", location, FRotator(0, 0, 0), scale, spawnParams, parent, meshPathCylinder);

    // Plug
    float plugWidth = cableCurrentX;
}

std::tuple<TArray<FVector>, TArray<FRotator>> ModelEVCharger::BuildCircularCableLocations(float totalLength, float circularAngleDegree, FVector startLocation, float stepAngleDegree, float unitLength)
{
    TArray<FVector> locations;
    TArray<FRotator> rotations;

    for (float angleDegree = stepAngleDegree; angleDegree <= circularAngleDegree; angleDegree += stepAngleDegree)
    {
        float X = unitLength * FMath::Sin(FMath::DegreesToRadians(angleDegree));
        float Z = unitLength * FMath::Cos(FMath::DegreesToRadians(angleDegree));
        FVector location = FVector(X + startLocation.X, startLocation.Y, startLocation.Z - Z);
        startLocation = location;
        FRotator rotation = FRotator(angleDegree, 0, 0);
        locations.Add(location);
        rotations.Add(rotation);
    }

    return {locations, rotations};
}