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

public:

	void SetSaveData();

	void GetSaveData();

private:

	TArray<class AMCWorldChunk*> SpawnedChunksRefs;
	TArray<FIntVector> SpawnedChunksCoords;	
};
