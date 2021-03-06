// Fill out your copyright notice in the Description page of Project Settings.


#include "GameObjects/Terrain/TerrainManager.h"
#include "GameObjects/Terrain/TerrainVoxel.h"
#include "Characters/NPC/NPCFactory.h"
#include "Utils/RNG.h"

ATerrainManager::ATerrainManager()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ATerrainManager::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	if (Seed != TNumericLimits<int32>::Max())
	{
		FRNG::Global().SetSeed(Seed);
	}
	CubeCountXYSquared = CubeCountXY * CubeCountXY;
	CubeLengthHalf = CubeLength / 2.0f;
	VoxelLength = static_cast<float>(CubeCountXY * CubeLength);
	Terrains.Reserve(RenderRadius * CubeCountXY);
}

void ATerrainManager::BeginPlay()
{
	Super::BeginPlay();

	check(NPCFactory);
}

void ATerrainManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (UpdatedPosition())
	{
		UpdateTerrains();
	}
}

bool ATerrainManager::UpdatedPosition()
{
	const FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	const int32 FloorX = FMath::Floor(PlayerLocation.X / static_cast<int32>(VoxelLength));
	const int32 FloorY = FMath::Floor(PlayerLocation.Y / static_cast<int32>(VoxelLength));

	// If the player moves to the other cube
	if (PlayerAtCubeX != FloorX || PlayerAtCubeY != FloorY)
	{
		PlayerAtCubeX = FloorX;
		PlayerAtCubeY = FloorY;
		return true;
	}

	return false;
}

void ATerrainManager::UpdateTerrains()
{
	AddChunk();
	RemoveChunk();
}

void ATerrainManager::AddChunk()
{	
	// Spawn Cubes within render radius
	for (int32 Y = PlayerAtCubeY - RenderRadius; Y < PlayerAtCubeY + RenderRadius; ++Y)
	{
		for (int32 X = PlayerAtCubeX - RenderRadius; X < PlayerAtCubeX + RenderRadius; ++X)
		{
			// Location of the candidate voxel to spawn
			const FVector VoxelLocation{ static_cast<float>(X * VoxelLength), static_cast<float>(Y * VoxelLength), 0.0f };

			// If this location is not in the TerrainLocations map
			if (!TerrainLocations.Contains(VoxelLocation))
			{
				SpawnVoxel(VoxelLocation, X, Y);
				Terrains.Emplace(SpawnedVoxel);
				TerrainLocations.Emplace(VoxelLocation, false);
			}
		}
	}
}

void ATerrainManager::RemoveChunk()
{
	// Remove cubes out of radius range
	const FVector PlayerLocation = GetWorld()->GetFirstPlayerController()->GetPawn()->GetActorLocation();
	const FVector2D PlayerLocation2D{ PlayerLocation.X, PlayerLocation.Y };
	for (int32 i = 0; i < Terrains.Num();)
	{
		const FVector2D VoxelLocation2D{ Terrains[i]->GetActorLocation().X, Terrains[i]->GetActorLocation().Y };
		const float DistanceToPlayer = FVector2D::Distance(VoxelLocation2D, PlayerLocation2D);
		if (DistanceToPlayer > RenderRadius * 2 * VoxelLength)
		{
			const FVector VoxelLocation = Terrains[i]->GetActorLocation() * FVector(1.0f, 1.0f, 0.0f);
			TerrainLocations.Remove(VoxelLocation);

			Swap(Terrains[i], Terrains.Last());
			GetWorld()->DestroyActor(Terrains.Last());
			Terrains.Pop();
		}
		else
		{
			++i;
		}
	}
}
