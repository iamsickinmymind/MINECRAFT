// All rights reserved Dominik Pavlicek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MCSaveGame.generated.h"

USTRUCT(BlueprintType)
struct FSaveData
{
	GENERATED_BODY()

	FSaveData()
	{
	};

protected:

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

	// TODO: Replace with struct
	bool SetSaveData(TArray<class AMCWorldChunk*> SpawnedChunksToSave, TArray<FIntVector> SpawnCoordsToSave, TArray<FVector> SpawnLocationsToSave, FVector PlayerPositionToSave, FVector2D LastKnownPlayerPosToSave, TArray<FVector> _DeletedBlocksLocations, TMap<FVector, int32> _SpawnedBlocksMapping);

	bool GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad, FVector &SpawnPlayerPosition, FVector2D &LastKnownPlayerPosToLoad, TArray<FVector> &SpawnDeletedBlocksLocations, TMap<FVector, int32> &SpawnSpawnedBlocksMapping);

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

	UPROPERTY(SaveGame)
	TMap<FVector, int32> SpawnedBlocksMapping;

	FString SaveSlotName;
};
