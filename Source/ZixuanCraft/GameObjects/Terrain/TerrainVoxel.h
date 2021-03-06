// Fill out your copyright notice in the Description page of Project Settings.
// TerrainVoxel.h
// Author: Zixuan Shi
// Game object with procedural mesh component, holding a portion of cubes

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameObjects/ObjectEnums.h"
#include "MeshSection.h"
#include "Utils/TypeDefs.h"
#include "Utils/RNG.h"
#include "TerrainVoxel.generated.h"

class ASpawnableCharacter;
class UProceduralMeshComponent;
class ATerrainManager;

/**
 * Voxel holding the terrain cubes meshes
 */
UCLASS()
class ZIXUANCRAFT_API ATerrainVoxel : public AActor
{
	GENERATED_BODY()

private:
	/** All the cubes info been managed by this TerrainVoxel */
	TArray<EObjectType> AllCubes;

	/** Stores Perlin-Noise Results for generating the cubes */
	TArray<int32> NoiseResult;

	/** Mesh renderer component that magically makes my game runs at 90 FPS */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UProceduralMeshComponent* ProceduralMeshComponent = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	ATerrainManager* TerrainManager = nullptr;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	TArray<ASpawnableCharacter*> SpawnedObjects;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 VoxelX = 0;

	UPROPERTY(BlueprintReadonly, meta = (ExposeOnSpawn = "true", AllowPrivateAccess = "true"))
	int32 VoxelY = 0;

public:	
	ATerrainVoxel();

	virtual void OnConstruction(const FTransform& Transform) override final;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override final;

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void ModifyCube(FVector CubeLocation, FVector SpawnLootLocation, EObjectType NewType);

private:
	/** Create voxel mesh information and determine the type of a cube */
	void GenerateChunk();
	void UpdateMesh();
	TArray<FMeshSection> GenerateMeshSections();
	void UpdateSingleCube(int32 X, int32 Y, int32 Z, TArray<FMeshSection>& MeshSections);
	void RegenerateMesh(const TArray<FMeshSection>& MeshSections);
	void ApplyMaterials();

	/** Vertices */
	void HandleNonEmptyCube(int32 X, int32 Y, int32 Z, const EObjectType ObjectType, TArray<FMeshSection>& MeshSections);
	void AddVertices(TArray<FVector>& Vertices, TArray<FVector>& Normals, int32 X, int32 Y, int32 Z, int32 CubeSideIndex);
	
	/** Calculate perlin noise value for terrain generation */
	void CalculateNoise();
};
