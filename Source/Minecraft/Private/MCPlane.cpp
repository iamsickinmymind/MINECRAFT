// Fill out your copyright notice in the Description page of Project Settings.


#include "MCPlane.h"
#include <Components/InstancedStaticMeshComponent.h>

// Sets default values
AMCPlane::AMCPlane()
{
	PrimaryActorTick.bCanEverTick = true;
	BlockPiece = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("Cube"));
	static ConstructorHelpers::FObjectFinder<UStaticMesh> MCCube(TEXT("/Game/0_MC/Mesh/SM_MCCube"));
	BlockPiece->SetStaticMesh(MCCube.Object);

	BlockPiece->AttachTo(GetRootComponent());

	bPlaneSpawned = false;
	Lenght = Width = 128;
	TileSize = 100;
}

void AMCPlane::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnCubes();
}

void AMCPlane::SetPlaneSpawned(const bool NewValue)
{
	bPlaneSpawned = NewValue;
}

void AMCPlane::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AMCPlane::SpawnCubes()
{
	if (IsPlaneSpawned())
	{
		BlockPiece->ClearInstances();
		SetPlaneSpawned(false);
	}

	if (BlockPiece->GetStaticMesh() && !IsPlaneSpawned())
	{
		for (int i = 1; i < Lenght; i++)
		{
			int32 SpawnSizeX = i * TileSize;

			for (int j = 1; j < Width; j++)
			{
				int32 SpawnSizeY = j * TileSize;

				FTransform SpawnTransform;
				FVector SpawnVector = FVector(SpawnSizeX, SpawnSizeY, 0);
				SpawnTransform.SetLocation(SpawnVector);
				BlockPiece->AddInstance(SpawnTransform);
			}
		}

		SetPlaneSpawned(true);
	}
}

