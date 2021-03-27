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
	bool SetSaveData(TArray<class AMCWorldChunk*> SpawnedChunksToSave, TArray<FIntVector> SpawnCoordsToSave, TArray<FVector> SpawnLocationsToSave, FVector PlayerPositionToSave, FVector2D LastKnownPlayerPosToSave);

	bool GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad, FVector &SpawnPlayerPosition, FVector2D &LastKnownPlayerPosToLoad);

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

	FString SaveSlotName;
};
