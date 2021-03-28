// Fill out your copyright notice in the Description page of Project Settings.


#include "MCPlayerController.h"
#include "UObject/ConstructorHelpers.h"
#include <Engine/StaticMesh.h>
#include "MCWorldChunk.h"
#include <MCSaveGame.h>
#include <Blueprint/UserWidget.h>

#ifdef WITH_EDITOR
#include "DrawDebugHelpers.h"
#endif WITH_EDITOR
#include "PhysicalMaterials/PhysicalMaterial.h"

AMCPlayerController::AMCPlayerController()
{
	ChunksSize = 1200.f;
	LastKnownPlayerChunkCoord = FVector2D(0, 0);
	RenderRange = 3;
	ChunkDepth = 1;
	ChunkArea = 5;
	VoxelSize = 100;
	NoiseDensity = 0.00055;
	NoiseScale = 4;
	
	Range = 200.f;
	DPS = 2.f;
	LastDigTime = 0;
	LastHitBox = FBox(FVector::ZeroVector, FVector::ZeroVector);
	HitCounter = 0;
	LastHitInstanceIndex = 0;

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshFinder(TEXT("/Game/0_MC/Mesh/SM_MCCube"));
	ChunkMesh = StaticMeshFinder.Object ? StaticMeshFinder.Object : nullptr;
	
	LastKnownPlayerChunkCoord = FVector2D(0,0);

	WorldSeed = FMath::RandRange(1, 1658);
	PlayerAction = EPlayerAction::EPA_Playing;

	DiggingDifficulty.Add(Grass, 2);
	DiggingDifficulty.Add(Snow, 1);
	DiggingDifficulty.Add(Stone, 8);
}

void AMCPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (DefaultWidgetClass)
	{
		DefaultWidget = CreateWidget<UUserWidget>(this, DefaultWidgetClass);
		if (DefaultWidget)
		{
			DefaultWidget->AddToViewport();
		}
	}

	SpawnChunks();
}

void AMCPlayerController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	// TODO
	// Check PlayerAction
	// If Digging Raycast from ViewPoint and process FHitResult
	// If Building Raycast from ViewPoint and process SpawnNewCube
	if (CanDig() || CanBuild())
	{
		// Use universal tracing

		if (GetPawn())
		{
			FHitResult HitResult;
			FVector ViewLoc;
			FRotator ViewRot;
			GetPlayerViewPoint(ViewLoc, ViewRot);

			FVector EndLoc;
			EndLoc = ViewRot.Vector();

			FVector RaycastEnd = (Range * EndLoc) + ViewLoc;

			FCollisionQueryParams CollisionQueryParams;
				CollisionQueryParams.AddIgnoredActor(this);
				CollisionQueryParams.AddIgnoredActor(GetPawn());
				CollisionQueryParams.bReturnPhysicalMaterial = true;
			FCollisionResponseParams CollisionResponsParams;

			#ifdef WITH_EDITOR
			FVector _RaycastStart = ViewLoc;
			FVector _RaycastEnd = RaycastEnd;
			DrawDebugLine(GetWorld(), _RaycastStart, _RaycastEnd, FColor::Red, false, 3, 0, 2.f);
			#endif WITH_EDITOR

			if (GetWorld()->LineTraceSingleByChannel(HitResult, ViewLoc, RaycastEnd, ECC_Visibility, CollisionQueryParams, CollisionResponsParams))
			{

				#ifdef WITH_EDITOR
				DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 16.f, 8, FColor::Red, false, 1);
				#endif WITH_EDITOR

				// Use tracing results based on player action
				if (CanDig())
				{
					LastDigTime = GetWorld()->GetTimeSeconds();

					// destroy hit instance if hit enough
					if (UInstancedStaticMeshComponent* HitComp = Cast<UInstancedStaticMeshComponent>(HitResult.GetComponent()))
					{
						auto HitBox = FBox(HitResult.Location, HitResult.TraceEnd);
						auto HitCubes = HitComp->GetInstancesOverlappingBox(HitBox);

						auto HitMaterial = HitResult.PhysMaterial;
						
						if
						(
							LastHitPhysMat == nullptr || // <------ for the first hit the LastHitPhysMat is not defined
							((LastHitPhysMat->GetName() == HitMaterial->GetName()) && (LastHitInstanceIndex == HitCubes[0])) || // <------ compare LastHitPhysMat AND indexes
							((LastHitPhysMat->GetName() == HitMaterial->GetName()) && (HitBox == LastHitBox)) // <-------- compare HitBoxes
						)
						{
							LastHitInstanceIndex = HitCubes[0];
							HitCounter++;
							LastHitPhysMat = HitMaterial;
							// No need to update LastHitBox because for it is roughly the same

							UE_LOG(LogTemp, Warning, TEXT("HitCounter: %d for %s"), HitCounter, *HitMaterial->GetName())

							// TODO
							// Replace LastHitBox with hitInstanceIndex

							if (HitCounter >= DiggingDifficulty.FindRef(HitMaterial->GetName()))
							{
								if (HitCubes.Num() >= 1)
								{
									HitComp->RemoveInstance(HitCubes[0]);

									FTransform DeletingInstanceTransform;
									HitComp->GetInstanceTransform(HitCubes[0], DeletingInstanceTransform);

									// adding location of deleted block to array of all deleted blocks
									DeletedCubesLocations.Add(DeletingInstanceTransform.GetLocation());

									// Delete values because cube was hit for its last time
									LastHitPhysMat = nullptr;
									LastHitBox = FBox();
									HitCounter = 0;

									// Save game after deleting a cube.
									SaveGame();

									// This may delete multiple cubes if hit on edge. Precision not guaranteed therefore avoided
									/*
									for (auto Itr : HitCubes)
									{
										HitComp->RemoveInstance(Itr);
									}
									*/
								}
							}
						}
						else
						{
							// If we hit different block or different material reset values
							HitCounter = 0;
							LastHitPhysMat = HitMaterial;
							LastHitBox = HitBox;
						}
					}
				}

				if (CanBuild())
				{
					// spawn block instance if possible
				}
			}
		}

		
	}
	
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

void AMCPlayerController::EscapePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("ESCAPE PRESSED - CONTROLLER"))

	if (MainMenu)
	{
		RemoveHUD();
		return;
	}

	if (MainMenuClass)
	{
		MainMenu = CreateWidget<UUserWidget>(this, MainMenuClass);
		if (MainMenu)
		{
			MainMenu->bIsFocusable = true;
			MainMenu->SetFocus();

			bShowMouseCursor = true;
			
			MainMenu->AddToViewport(0);
			FInputModeUIOnly NewInputMode;

			SetInputMode(NewInputMode);
		}
	}
}

void AMCPlayerController::ExitGame()
{
	if (SaveGame())
	{
		FGenericPlatformMisc::RequestExitWithStatus(false, 0);
	}
}

bool AMCPlayerController::SaveGame()
{
	if (!GetPawn())
	{
		return false;
	}
	UMCSaveGame* NewSaveGame = NewObject<UMCSaveGame>();
	if (NewSaveGame)
	{
		return NewSaveGame->SetSaveData(SpawnedChunksRefs, SpawnedChunksCoords, SpawnedChunksLocations, GetPawn()->GetActorLocation(), LastKnownPlayerChunkCoord, DeletedCubesLocations);
	}

	return false;
}

bool AMCPlayerController::LoadGame()
{
	// TODO 
	// fix crashing after loading
	// fix some chunks not spawning

	if (!GetPawn())
	{
		return false;
	}
	UMCSaveGame* NewLoadGame = NewObject<UMCSaveGame>();
	if (NewLoadGame)
	{
		FVector NewPlayerPosition(0);

		TArray<class AMCWorldChunk*> TempSpawnedChunksRefs;
		TArray<FIntVector> TempSpawnedChunksCoords;
		TArray<FVector> TmpSpawnedChunksLocations;
		FVector2D TempLastKnownPlayerChunkCoord;
		TArray<FVector> TempDeletedBlocksLocations;

		if( NewLoadGame->GetSaveData(TempSpawnedChunksRefs, TempSpawnedChunksCoords, TmpSpawnedChunksLocations, NewPlayerPosition, TempLastKnownPlayerChunkCoord, TempDeletedBlocksLocations))
		{
			// TODO: reload the world here

			// Destroy all current chunks
			for (auto Itr : SpawnedChunksRefs)
			{
				if (Itr == Cast<AMCWorldChunk>(Itr))
				{
					Itr->Destroy();
				}
			}
			
			// Clean arrays
			SpawnedChunksRefs.Empty();
			SpawnedChunksCoords.Empty();
			SpawnedChunksLocations.Empty();
			DeletedCubesLocations.Empty();

			SpawnedChunksRefs = TempSpawnedChunksRefs;
			SpawnedChunksCoords = TempSpawnedChunksCoords;
			SpawnedChunksLocations = TmpSpawnedChunksLocations;
			DeletedCubesLocations = TempDeletedBlocksLocations;

			for (int32 i = 0; i < SpawnedChunksRefs.Num(); i++)
			{
				if (SpawnedChunksRefs[i])
				{
					FRotator TempRot(0);
					FActorSpawnParameters ChunkSpawnParams;
					ChunkSpawnParams.Owner = this;
					AMCWorldChunk* LoadedChunk = GetWorld()->SpawnActor<AMCWorldChunk>(WorldChunkClass, SpawnedChunksLocations[i], TempRot, ChunkSpawnParams);
				}
			}

			// SetActorLocation must be the last thing to do
			GetPawn()->SetActorLocation(NewPlayerPosition);

			return true;
		}
	}
	
	return false;
}

void AMCPlayerController::RemoveHUD()
{
	MainMenu->RemoveFromParent();
	MainMenu = nullptr;
	FInputModeGameOnly NewInputMode;

	bShowMouseCursor = false;

	SetInputMode(NewInputMode);
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

bool AMCPlayerController::CanDig() const
{
	// Can dig if PlayerAction is digging AND elapsed enough time from last dig to prevent spaming
	return (PlayerAction == EPlayerAction::EPA_Digging) && ((LastDigTime + DPS) <= GetWorld()->GetTimeSeconds());
}

void AMCPlayerController::DigStarted()
{
	if (PlayerAction != EPlayerAction::EPA_Digging)
	{
		PlayerAction = EPlayerAction::EPA_Digging;
	}
	else
	{
		DigStopped();
	}
}

void AMCPlayerController::DigStopped()
{
	// If I exit digging by starting building I dont need to call this function
	if (PlayerAction == EPlayerAction::EPA_Digging)
	{
		PlayerAction = EPlayerAction::EPA_Playing;
	}
}

void AMCPlayerController::BuildingStarted()
{
	if (PlayerAction != EPlayerAction::EPA_Building)
	{
		PlayerAction = EPlayerAction::EPA_Building;
	}
	else
	{
		BuildingStopped();
	}
}

void AMCPlayerController::BuildingStopped()
{
	if (PlayerAction == EPlayerAction::EPA_Building)
	{
		PlayerAction = EPlayerAction::EPA_Playing;
	}
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

			
			ChunkCoords = FIntVector(GetPlayerChunk().X + TempX, GetPlayerChunk().Y + TempY, 0);

			if ( (FVector(TempX * ChunksSize, TempY * ChunksSize, 0).Size() <= (RenderRange * ChunksSize)) && !(SpawnedChunksCoords.Contains(ChunkCoords))) //<----------- this one gave me nightmares; check whether epected spawnpoint is within render range && this index is not already occupied by any other chunk
			{
				if(GetWorld())
				{
					FVector TempLoc = FVector((GetPlayerChunk().X + TempX) * ChunksSize, (GetPlayerChunk().Y + TempY) * ChunksSize, 0);
					SpawnedChunksLocations.Add(TempLoc);

					// TODO: Remove from iterations and spawn after iterations to iterate as fast as possible
					
					FRotator TempRot = FRotator(0);
					FActorSpawnParameters TempParams;
					TempParams.bNoFail = true;
					TempParams.Owner = this;
					TempParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

					AActor* TempChunk = GetWorld()->SpawnActor<AActor>(WorldChunkClass, TempLoc, TempRot, TempParams);
					if (TempChunk)
					{
						if (AMCWorldChunk* NewChunk = Cast<AMCWorldChunk>(TempChunk))
						{
							SpawnedChunksRefs.Add(NewChunk);
							SpawnedChunksCoords.Add(ChunkCoords);

							// TODO: Here I wanted to create some Init functions to pass variables
							// This wont work because it is called after BeginPlay
							// NewChunk->InitializeWorldChunk(ChunkMesh, ChunksSize, ChunkDepth, VoxelSize, NoiseDensity, NoiseScale, _3DNoiseDensity, _3DNoiseCutoff);
							// NewChunk->Init(ChunkMesh, ChunkArea, ChunkDepth, VoxelSize, NoiseDensity, NoiseScale, _3DNoiseDensity, _3DNoiseCutoff);
						}
					}
				}
			}
		}

	}

	UE_LOG(LogTemp, Error, TEXT("Finished calculating spawn locations"))

	
	/************************************************************************/
	/*                      SPAWN CHUNKS ON LOCATIONS                       */
	/************************************************************************/
	/*
	/* This idea wont work because of SpawnedChunksCoords.
	if (GetWorld())
	{
		for (auto ItrLoc : SpawnedChunksLocations)
		{
			FRotator TempRot;
			FActorSpawnParameters TempParams;
			TempParams.bNoFail = true;
			TempParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;

			AActor* TempChunk = GetWorld()->SpawnActor<AActor>(WorldChunkClass, ItrLoc, TempRot, TempParams);
			if (TempChunk)
			{
				if (AMCWorldChunk* NewChunk = Cast<AMCWorldChunk>(TempChunk))
				{
					SpawnedChunksRefs.Add(NewChunk);
					SpawnedChunksCoords.Add(ChunkCoords);

					// TODO: Here I wanted to create some Init functions to pass variables
					// This will cause issues for loading, too.
					// NewChunk->InitializeWorldChunk(ChunkMesh, ChunksSize, ChunkDepth, VoxelSize, NoiseDensity, NoiseScale, _3DNoiseDensity, _3DNoiseCutoff);
					NewChunk->Init(ChunkMesh, ChunkArea, ChunkDepth, VoxelSize, NoiseDensity, NoiseScale, _3DNoiseDensity, _3DNoiseCutoff);
				}
			}
		}
	}
	*/
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
				// TODO
				// Rather than deleting chunks use ClearInstances() for those chunks
				// It might be better for performance and then just spawn new chunks atop those empty old ones
				SpawnedChunksRefs[i]->Destroy();
				SpawnedChunksRefs.RemoveAt(i);
				SpawnedChunksCoords.RemoveAt(i);
				SpawnedChunksLocations.RemoveAt(i);
			}
		}
	}

	
}