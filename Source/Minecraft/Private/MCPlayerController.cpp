// Fill out your copyright notice in the Description page of Project Settings.


#include "MCPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include <Engine/StaticMesh.h>
#include "MCWorldChunk.h"

AMCPlayerController::AMCPlayerController()
{
	ChunksSize = 3300.f;
	LastKnownPlayerChunkCoord = FVector2D(0, 0);
	RenderRange = 3;
	ChunkDepth = 1;
	ChunkArea = 16;
	VoxelSize = 100;
	NoiseDensity = 0.00055;
	NoiseScale = 4;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshFinder(TEXT("/Game/0_MC/Mesh/SM_MCCube"));
	ChunkMesh = StaticMeshFinder.Object ? StaticMeshFinder.Object : nullptr;
	
	LastKnownPlayerChunkCoord = FVector2D(0,0);

	WorldSeed = FMath::RandRange(1, 1658);
}

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SpawnChunks();
}

void AMCPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// Check every tick whether player moved from hist last chunk.
	// Smaller chunks = heavier impact on performance, smoother visibility & spawns

	if (GetPawn() && (GetPlayerChunk().X != LastKnownPlayerChunkCoord.X || GetPlayerChunk().Y != LastKnownPlayerChunkCoord.Y))
	{
		SetLastKnownChunkCoords(FVector2D(GetPlayerChunk().X, GetPlayerChunk().Y));

		PlayerMoved();
	}
}

void AMCPlayerController::PlayerMoved()
{
	DestroyChunks();

	SpawnChunks();
}

FVector2D AMCPlayerController::GetPlayerChunk() const
{
	FVector2D ReturnValue = FVector2D(0,0);

	if (GetPawn())
	{
		FVector ScaledPlayerPos = GetPawn()->GetActorLocation() / ChunksSize;
		float PLayerPosX = ScaledPlayerPos.X;
		float PlayerPosY = ScaledPlayerPos.Y;

		ReturnValue.X = FMath::RoundToInt(ScaledPlayerPos.X);
		ReturnValue.Y = FMath::RoundToInt(ScaledPlayerPos.Y);
	}
	
	return ReturnValue;
}

void AMCPlayerController::SetLastKnownChunkCoords(FVector2D NewCoords)
{
	LastKnownPlayerChunkCoord.X = NewCoords.X;
	LastKnownPlayerChunkCoord.Y = NewCoords.Y;
}

void AMCPlayerController::SpawnChunks()
{

/************************************************************************/
/*                        CALCULATE LOCATIONS                           */
/************************************************************************/
	for (int AreaX = (RenderRange * (-1)); AreaX < RenderRange; AreaX++)
	{

		int32 TempX = AreaX;

		for (int AreaY = (RenderRange * (-1)); AreaY < RenderRange; AreaY++)
		{

			int32 TempY = AreaY;

			/************************************************************************/
			/*                      SPAWN CHUNKS ON LOCATIONS                       */
			/************************************************************************/
			ChunkCoords = FIntVector(GetPlayerChunk().X + TempX, GetPlayerChunk().Y + TempY, 0);

			if ( (FVector(TempX * ChunksSize, TempY * ChunksSize, 0).Size() <= (RenderRange * ChunksSize)) && !(SpawnedChunksCoords.Contains(ChunkCoords))) //<----------- this one gave me nightmares; check whether epected spawnpoint is within render range && this index is not already occupied by any other chunk
			{
				if(GetWorld())
				{
					FVector TempLoc = FVector((GetPlayerChunk().X + TempX) * ChunksSize, (GetPlayerChunk().Y + TempY) * ChunksSize, 0);
					FRotator TempRot;
					FActorSpawnParameters TempParams;
					TempParams.bNoFail = true;
					TempParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

					AActor* TempChunk = GetWorld()->SpawnActor<AActor>(WorldChunkClass, TempLoc, TempRot, TempParams);
					if (TempChunk)
					{
						if (AMCWorldChunk* NewChunk = Cast<AMCWorldChunk>(TempChunk))
						{
							SpawnedChunksRefs.Add(NewChunk);
							SpawnedChunksCoords.Add(ChunkCoords);

							// TODO: Here I wanted to create some Init functions to pass variables
							// NewChunk->InitializeWorldChunk(ChunkMesh, ChunksSize, ChunkDepth, VoxelSize, NoiseDensity, NoiseScale, _3DNoiseDensity, _3DNoiseCutoff);
							// NewChunk->SpawnWorldChunk(ChunkMesh, ChunksSize, ChunkDepth, VoxelSize, NoiseDensity, NoiseScale, _3DNoiseDensity, _3DNoiseCutoff);
						}
					}
				}
			}
		}

	}
}

void AMCPlayerController::DestroyChunks()
{
	for (int i = 0; i < SpawnedChunksCoords.Num(); i++)
	{
		// Whether coords of the chunk are further away than RenderRange
		float VectorDistance = FVector::Distance
		(
			FVector(SpawnedChunksCoords[i].X * ChunksSize, SpawnedChunksCoords[i].Y * ChunksSize, 0), GetPawn()->GetActorLocation()
		);

		if (VectorDistance > (RenderRange * ChunksSize))
		{
			if (SpawnedChunksRefs[i]) 
			{
				SpawnedChunksRefs[i]->Destroy();
				SpawnedChunksRefs.RemoveAt(i);
				SpawnedChunksCoords.RemoveAt(i);
			}
		}
	}
}