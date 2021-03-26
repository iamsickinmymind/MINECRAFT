// All rights reserved Dominik Pavlicek


#include "MCSaveGame.h"
#include <Kismet/GameplayStatics.h>

UMCSaveGame::UMCSaveGame()
{
	PlayerName = TEXT("MINECRAFT_PLAYER");
	SaveSlotName = TEXT("MINECRAFT_SAVE");
}

// TODO: FStruct SaveGameData
bool UMCSaveGame::SetSaveData(TArray<class AMCWorldChunk*> SpawnedChunksToSave, TArray<FIntVector> SpawnCoordsToSave, TArray<FVector> SpawnLocationsToSave, FVector PlayerPositionToSave)
{
	if (UMCSaveGame* SaveGameInstance = Cast<UMCSaveGame>(UGameplayStatics::CreateSaveGameObject(UMCSaveGame::StaticClass())))
	{
		// Set data on the savegame object.
		SaveGameInstance->PlayerName = TEXT("MINECRAFT_PLAYER");

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
			UE_LOG(LogTemp, Warning, TEXT("SAVE OK"))
			return true;
		}
		else
		{
			return false;
		}
	}

	return false;
}

void UMCSaveGame::GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad, FVector &SpawnPlayerPosition)
{
		
}
