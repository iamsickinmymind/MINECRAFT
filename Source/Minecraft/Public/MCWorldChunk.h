// All rights reserved Dominik Pavlicek

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCWorldChunk.generated.h"

UCLASS()
class MINECRAFT_API AMCWorldChunk : public AActor
{
	GENERATED_BODY()
	
public:	
	AMCWorldChunk();
	AMCWorldChunk(class UStaticMesh* NewBoxMesh, int32 NewArea, int32 NewDepth, int32 NewVoxelSize, float NewNoiseDensity, int32 NewNoiseScale, float New3DNoiseDensity, float New3DNoiseCutOff);

	//void InitializeWorldChunk(class UStaticMesh* NewBoxMesh, int32 NewArea, int32 NewDepth, int32 NewVoxelSize, float NewNoiseDensity, int32 NewNoiseScale, float New3DNoiseDensity, float New3DNoiseCutOff);

	void Init(class UStaticMesh* NewBoxMesh, int32 NewArea, int32 NewDepth, int32 NewVoxelSize, float NewNoiseDensity, int32 NewNoiseScale, float New3DNoiseDensity, float New3DNoiseCutOff);

	void PostActorCreated() override;

	UFUNCTION(BlueprintCallable, Category = "Spawn")
	void ForceNewInstance(FVector SpawnLoc, int32 BlockIndex);

protected:

	virtual void BeginPlay() override;

	void SpawnWorldChunk();

	//void InitializeWorldChunk();
	bool Get3DNoiseZ(const float InNoiseCutof, const float InNoiseDensity);
	void SpawnCubeInstance();
	bool IsBlockDeleted(FVector &CheckLocation) const;
	void AddVoxelInstanceOfType(class UInstancedStaticMeshComponent* InstanceType);

#pragma region PROTECTED_VARIABLES

	/** Mesh used for instancing*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings", meta=(ExposeOnSpawn="true"))
	class UStaticMesh* BoxMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TMap<int32, class UMaterialInstance*> MaterialsMapping;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Dimensions", meta=(DisplayName="Area(2n-1)", ClampMin = 0, UIMin = 0, ExposeOnSpawn="true"))
	int32 Area;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Settings|Dimensions", meta=(DisplayName="Depth(2n-1)", ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true"))
	int32 Depth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Dimensions", meta = (ClampMin=0, UIMin=0, ExposeOnSpawn = "true"))
	int32 VoxelSize;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true"))
	float NoiseDensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true"))
	int32 NoiseScale;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true", DisplayName="3D Noise Density"))
	float _3DNoiseDensity;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true", DisplayName="3D CutOff Noise Treshold"))
	float _3DNoiseCutOff;

	/** Higher values = snow*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0))
	float SnowTreshold;

	/** Lower values = Grass*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0))
	float GrassTreshold;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0))
	float DirtTreshold;

	int32 LocalVoxelPos_X;
	int32 LocalVoxelPos_Y;
	int32 LocalVoxelPos_Z;
	int32 WorldVoxelPos_X;
	int32 WorldVoxelPos_Y;
	int32 WorldVoxelPos_Z_PreNoise;
	int32 WorldVoxelPos_Z_Noised;

	UPROPERTY(VisibleAnywhere)
	class USceneComponent* TempRoot = nullptr;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings|Hidden")
	TArray<class UInstancedStaticMeshComponent*> InstancedBoxes;

	TArray<FVector> DeletedBlocksLocations;
	TMap<FVector, int32> PlayerSpawnedBlocksLocations;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings|Hidden")
	class UInstancedStaticMeshComponent* ManuallySpawnedGrass = nullptr;

#pragma endregion PROTECTED_VARIABLES
};
