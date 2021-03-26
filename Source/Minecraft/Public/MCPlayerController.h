// All rights reserved Dominik Pavlicek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MINECRAFT_API AMCPlayerController : public APlayerController
{
	GENERATED_BODY()

	AMCPlayerController();

	friend class AMinecraftCharacter;
	
public:

#pragma region PUBLIC_VARIABLES

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "World Chunk")
	int32 WorldSeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	TSubclassOf<class AActor> WorldChunkClass;

	/** Defines size of the chunk. Each chunk consists of individual boxes.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	float ChunksSize;

	/** Last Chunk player was on.*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "World Chunk|Settings")
	FVector2D LastKnownPlayerChunkCoord;

	/** How many chunks to render around player.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	int32 RenderRange;

	/** How many chunks to spawn in Z location.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	int32 ChunkDepth;

	/** How many chunks to spawn in XY locations.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	int32 ChunkArea;

	/** Size of each box. Unit: Centimeters.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	int32 VoxelSize;

	/** Defines scale of the noise..*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	int32 NoiseScale;

	/** Density of the noise.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	float NoiseDensity;

	/** Density of the noise.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	float _3DNoiseDensity;

	/** Density of the noise.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	float _3DNoiseCutoff;

	/** Static Mesh to be used for Chunk instanced Mesh*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "World Chunk|Settings")
	class UStaticMesh* ChunkMesh = nullptr;
	

#pragma endregion PUBLIC_VARIABLES

#pragma region PUBLIC_FUNCTIONS

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "World Chunk")
	FVector2D GetPlayerChunk() const;

	UFUNCTION(BlueprintCallable, Category = "World Chunk")
	void SetLastKnownChunkCoords(FVector2D NewCoords);

#pragma endregion PUBLIC_FUNCTIONS

protected:

#pragma region PROTECTED_FUNCTIONS

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaSeconds) override;

	UFUNCTION(BlueprintCallable, Category = "World Chunk")
	void SpawnChunks();

	UFUNCTION(BlueprintCallable, Category = "World Chunk")
	void DestroyChunks();

	/** Maybe create broadcasting event and replicate?*/
	UFUNCTION(BlueprintCallable, Category = "World Chunk")
	void PlayerMoved();

	UFUNCTION()
	void EscapePressed();

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void ExitGame();

	UFUNCTION(BlueprintCallable, Category = "SaveLoadGame")
	bool SaveGame();

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void RemoveHUD();

#pragma endregion PROTECTED_FUNCTIONS

#pragma region PROTECTED_VARIABLES
	
	//TODO: Maybe use FMap where FIntVector is a key.
	TArray<class AMCWorldChunk*> SpawnedChunksRefs;
	TArray<FIntVector> SpawnedChunksCoords;
	TArray<FVector> SpawnedChunksLocations;
	FIntVector ChunkCoords;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> MainMenuClass;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "HUD")
	class UUserWidget* MainMenu = nullptr;

#pragma endregion PROTECTED_VARIABLES
};
