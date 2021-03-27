// All rights reserved Dominik Pavlicek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MCPlayerController.generated.h"

#define Snow = SurfaceType1;
#define Grass = SurfaceType2;
#define Stone = SurfaceType3;
#define Dirt = SurfaceType4;

UENUM(BlueprintType)
enum class EPlayerAction : uint8
{
	EPA_Playing		UMETA(DisplayName = "Playing"),
	EPA_Digging		UMETA(DisplayName = "Digging"),
	EPA_Building	UMETA(DisplayName = "Building"),

	EPA_default		 UMETA(Hidden)
};


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

	UFUNCTION(BlueprintPure, BlueprintCallable, Category ="Player")
	FORCEINLINE EPlayerAction GetPlayerAction() const
	{
		return PlayerAction;
	}

	UFUNCTION(BlueprintPure, BlueprintCallable, Category ="Player")
	FORCEINLINE bool CanDig() const {return PlayerAction == EPlayerAction::EPA_Digging; };

	UFUNCTION(BlueprintPure, BlueprintCallable, Category = "Player")
	FORCEINLINE bool CanBuild() const { return PlayerAction == EPlayerAction::EPA_Building; };

	void DigStarted();
	void DigStopped();

	void BuildingStarted();
	void BuildingStopped();

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
	
	UFUNCTION(BlueprintCallable, Category = "SaveLoadGame")
	bool LoadGame();

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
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "HUD")
	TSubclassOf<class UUserWidget> DefaultWidgetClass;

	class UUserWidget* MainMenu = nullptr;
	class UUserWidget* DefaultWidget = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "SaveLoad")
	TSubclassOf<class USaveGame> SaveGameClass;

	// Defines what action is player doing. This should be implemented in Player State tho
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Player")
	EPlayerAction PlayerAction;

	/** How far from players viewpoint is digging and building allowed.*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Player|Interaction", meta=(UIMin = 0, ClampMin=0))
	float Range;

	/** For each PhysicalMaterial define how many times player need to hit the Cube. If not defined default is 0; */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category ="Player|Interaction")
	TMap<FString, int32> DiggingDifficulty;

#pragma endregion PROTECTED_VARIABLES
};
