// All rights reserved Dominik Pavlicek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MCSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FChunkData
{
	GENERATED_BODY()

	FChunkData()
	{
		;
	};

	FChunkData(TArray<FVector> _DestroyedLocations)
	{
		DestroyedLocations = _DestroyedLocations;
	};

protected:

	TArray<FVector> DestroyedLocations;
};

/**
 * 
 */
UCLASS()
class MINECRAFT_API UMCSaveGame : public USaveGame
{
	GENERATED_BODY()

	UMCSaveGame();

public:

	// TODO: Replace with & to save transports (or is it dangerous for SaveGame?)
	bool SetSaveData(TArray<class AMCWorldChunk*> SpawnedChunksToSave, TArray<FIntVector> SpawnCoordsToSave, TArray<FVector> SpawnLocationsToSave, FVector PlayerPositionToSave, FVector2D LastKnownPlayerPosToSave, TArray<FVector> _DeletedBlocksLocations);

	bool GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad, FVector &SpawnPlayerPosition, FVector2D &LastKnownPlayerPosToLoad, TArray<FVector> &SpawnDeletedBlocksLocations);

	UFUNCTION(BlueprintCallable, Category = "TEST")
	FORCEINLINE bool CallFromBP() { return true; };

private:
	UPROPERTY(SaveGame)
	TArray<class AMCWorldChunk*> SpawnedChunksRefs;

	UPROPERTY(SaveGame)
	TArray<FIntVector> SpawnedChunksCoords;

	UPROPERTY(SaveGame)
	TArray<FVector> SpawnedChunksLocations;

	UPROPERTY(SaveGame)
	FVector PlayerPosition;

	UPROPERTY(SaveGame)
	FVector2D LastKnownPlayerPos;

	UPROPERTY(SaveGame)
	TArray<FVector> DeletedBlocksLocations;

	FString SaveSlotName;
};
