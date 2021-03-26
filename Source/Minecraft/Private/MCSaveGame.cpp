// All rights reserved Dominik Pavlicek


#include "MCSaveGame.h"
#include <Kismet/GameplayStatics.h>

UMCSaveGame::UMCSaveGame()
{
	PlayerName = TEXT("MINECRAFT_PLAYER");
	SaveSlotName = TEXT("MINECRAFT_SAVE");
}

// TODO: FStruct SaveGameData
void UMCSaveGame::SetSaveData(TArray<class AMCWorldChunk*> &SpawnedChunksToSave, TArray<FIntVector> &SpawnCoordsToSave, TArray<FVector> &SpawnLocationsToSave)
{
	if (UMCSaveGame* SaveGameInstance = Cast<UMCSaveGame>(UGameplayStatics::CreateSaveGameObject(UMCSaveGame::StaticClass())))
	{
		// Set data on the savegame object.
		SaveGameInstance->PlayerName = TEXT("MINECRAFT_PLAYER");

		SaveGameInstance->SpawnedChunksRefs.Empty();
		SaveGameInstance->SpawnedChunksCoords.Empty();
		SaveGameInstance->SpawnedChunksLocations.Empty();
		
		SaveGameInstance->SpawnedChunksRefs = SpawnedChunksToSave;;
		SaveGameInstance->SpawnedChunksCoords = SpawnCoordsToSave;;
		SaveGameInstance->SpawnedChunksLocations = SpawnLocationsToSave;

		// Save the data immediately.
		if (UGameplayStatics::SaveGameToSlot(SaveGameInstance, SaveSlotName, 0))
		{
			UE_LOG(LogTemp, Warning, TEXT("SAVE OK"))
		}
	}
}

void UMCSaveGame::GetSaveData(TArray<class AMCWorldChunk*> &WorlChunksToLoad, TArray<FIntVector> &SpawnCoordsToLoad, TArray<FVector> &SpawnLocationsToLoad)
{
	
}
