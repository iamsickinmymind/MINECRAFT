// All rights reserved Dominik Pavlicek


#include "MCSaveGame.h"
#include <Kismet/GameplayStatics.h>

UMCSaveGame::UMCSaveGame()
{
	SaveSlotName = TEXT("MINECRAFT_SAVE");
}

// TODO: FStruct SaveGameData
bool UMCSaveGame::SetSaveData(TArray<class AMCWorldChunk*> SpawnedChunksToSave, TArray<FIntVector> SpawnCoordsToSave, TArray<FVector> SpawnLocationsToSave, FVector PlayerPositionToSave, FVector2D LastKnownPlayerPosToSave)
{
	if (UMCSaveGame* SaveGameInstance = Cast<UMCSaveGame>(UGameplayStatics::CreateSaveGameObject(UMCSaveGame::StaticClass())))
	{
		// Set data on the savegame object.

		SaveGameInstance->SpawnedChunksRefs.Empty();
		SaveGameInstance->SpawnedChunksCoords.Empty();
		SaveGameInstance->SpawnedChunksLocations.Empty();
		SaveGameInstance->PlayerPosition = FVector(0);
		SaveGameInstance->LastKnownPlayerPos = FVector2D(0);
		
		SaveGameInstance->SpawnedChunksRefs = SpawnedChunksToSave;
		SaveGameInstance->SpawnedChunksCoords = SpawnCoordsToSave;
		SaveGameInstance->SpawnedChunksLocations = SpawnLocationsToSave;
		SaveGameInstance->PlayerPosition = PlayerPositionToSave;
		SaveGameInstance->LastKnownPlayerPos = LastKnownPlayerPosToSave;

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

bool UMCSaveGame::GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad, FVector &SpawnPlayerPosition, FVector2D &LastKnownPlayerPosToLoad)
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
			if (LoadGameInstance->SpawnedChunksRefs.Num() == 0) return false;
			WorlChunksToLoad = LoadGameInstance->SpawnedChunksRefs;

			if(LoadGameInstance->SpawnedChunksCoords.Num() == 0) return false;
			SpawnCoordsToLoad = LoadGameInstance->SpawnedChunksCoords;

			if(LoadGameInstance->SpawnedChunksLocations.Num() == 0) return false;
			SpawnLocationsToLoad = LoadGameInstance->SpawnedChunksLocations;

			SpawnPlayerPosition = LoadGameInstance->PlayerPosition;

			LastKnownPlayerPosToLoad = LoadGameInstance->LastKnownPlayerPos;

			return true;
		}
	}
		
	return false;
}
