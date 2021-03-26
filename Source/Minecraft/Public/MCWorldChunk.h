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

	void SpawnWorldChunk(class UStaticMesh* NewBoxMesh, int32 NewArea, int32 NewDepth, int32 NewVoxelSize, float NewNoiseDensity, int32 NewNoiseScale, float New3DNoiseDensity, float New3DNoiseCutOff);


protected:

	void SpawnWorldChunk();

	//void InitializeWorldChunk();
	bool Get3DNoiseZ(const float InNoiseCutof, const float InNoiseDensity);
	void SpawnCubeInstance();
	void AddVoxelInstanceOfType(class UInstancedStaticMeshComponent* InstanceType);

#pragma region PROTECTED_VARIABLES

	/** Mesh used for instancing*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Settings", meta=(ExposeOnSpawn="true"))
	class UStaticMesh* BoxMesh = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings")
	TMap<int32, class UMaterialInstance*> MaterialsMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Dimensions", meta=(DisplayName="Area(2n-1)", ClampMin = 0, UIMin = 0, ExposeOnSpawn="true"))
	int32 Area = 16;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Dimensions", meta=(DisplayName="Depth2n-1", ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true"))
	int32 Depth = 3;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Dimensions", meta = (ClampMin=0, UIMin=0, ExposeOnSpawn = "true"))
	int32 VoxelSize = 100;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true"))
	float NoiseDensity = 0.00055;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true"))
	int32 NoiseScale = 6;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true", DisplayName="3D Noise Density"))
	float _3DNoiseDensity = 0.01;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0, ExposeOnSpawn = "true", DisplayName="3D CutOff Noise Treshold"))
	float _3DNoiseCutOff = 0.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0))
	float SnowTreshold = 600;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Settings|Noise", meta = (ClampMin = 0, UIMin = 0))
	float GrassTreshold = 350;

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

#pragma endregion PROTECTED_VARIABLES
};
