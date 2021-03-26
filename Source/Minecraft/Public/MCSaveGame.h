// All rights reserved Dominik Pavlicek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MCSaveGame.generated.h"

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
	void SetSaveData(TArray<class AMCWorldChunk*> &SpawnedChunksToSave, TArray<FIntVector> &SpawnCoordsToSave, TArray<FVector> &SpawnLocationsToSave);

	void GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad);

private:

	TArray<class AMCWorldChunk*> SpawnedChunksRefs;
	TArray<FIntVector> SpawnedChunksCoords;
	TArray<FVector> SpawnedChunksLocations;

	FString SaveSlotName;
	FString PlayerName;
};
