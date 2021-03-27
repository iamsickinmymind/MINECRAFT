// All rights reserved Dominik Pavlicek


#include "MCSaveGame.h"
#include <Kismet/GameplayStatics.h>

UMCSaveGame::UMCSaveGame()
{
	// PlayerName = TEXT("MINECRAFT_PLAYER");
	SaveSlotName = TEXT("MINECRAFT_SAVE");
}

// TODO: FStruct SaveGameData
bool UMCSaveGame::SetSaveData(TArray<class AMCWorldChunk*> SpawnedChunksToSave, TArray<FIntVector> SpawnCoordsToSave, TArray<FVector> SpawnLocationsToSave, FVector PlayerPositionToSave)
{
	if (UMCSaveGame* SaveGameInstance = Cast<UMCSaveGame>(UGameplayStatics::CreateSaveGameObject(UMCSaveGame::StaticClass())))
	{
		// Set data on the savegame object.
		// SaveGameInstance->PlayerName = TEXT("MINECRAFT_PLAYER");

		SaveGameInstance->SpawnedChunksRefs.Empty();
		SaveGameInstance->SpawnedChunksCoords.Empty();
		SaveGameInstance->SpawnedChunksLocations.Empty();
		SaveGameInstance->PlayerPosition = FVector(0);
		
		SaveGameInstance->SpawnedChunksRefs = SpawnedChunksToSave;
		SaveGameInstance->SpawnedChunksCoords = SpawnCoordsToSave;
		SaveGameInstance->SpawnedChunksLocations = SpawnLocationsToSave;
		SaveGameInstance->PlayerPosition = PlayerPositionToSave;

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

bool UMCSaveGame::GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad, FVector &SpawnPlayerPosition)
{
	if (!(UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)))
	{
		UE_LOG(LogTemp, Error, TEXT("SaveGame Does Not Exist: %s"), *SaveSlotName)
		return false;
	}

	if (UMCSaveGame* LoadGameInstance = Cast<UMCSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0)))
	{
		if (LoadGameInstance)
		{
			UE_LOG(LogTemp, Warning, TEXT("LoadingChunks: %d"), LoadGameInstance->SpawnedChunksRefs.Num())
			UE_LOG(LogTemp, Warning, TEXT("LoadingRef: %d"), LoadGameInstance->SpawnedChunksCoords.Num())
			UE_LOG(LogTemp, Warning, TEXT("LoadingLocations: %d"), LoadGameInstance->SpawnedChunksLocations.Num())
			UE_LOG(LogTemp, Warning, TEXT("LoadedVector: %s"), *LoadGameInstance->PlayerPosition.ToString())
			
			WorlChunksToLoad = LoadGameInstance->SpawnedChunksRefs;
			SpawnCoordsToLoad = LoadGameInstance->SpawnedChunksCoords;
			SpawnLocationsToLoad = LoadGameInstance->SpawnedChunksLocations;
			SpawnPlayerPosition = LoadGameInstance->PlayerPosition;

			UE_LOG(LogTemp, Warning, TEXT("LoadedVector: %s"), *LoadGameInstance->PlayerPosition.ToString())

			return true;
		}
	}
		
	return false;
}
