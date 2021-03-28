// All rights reserved Dominik Pavlicek


#include "MCWorldChunk.h"
#include <Materials/MaterialInstance.h>
#include <SimplexNoiseBPLibrary.h>
#include <MCPlayerController.h> //OPTIMISE <--------- I dont like this circular deendency

// Sets default values
AMCWorldChunk::AMCWorldChunk()
{
	PrimaryActorTick.bCanEverTick = false;

	TempRoot = CreateDefaultSubobject<USceneComponent>(TEXT("NewRoot"));
	SetRootComponent(TempRoot);

	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceFinder_Snow(TEXT("/Game/0_MC/Materials/MI_Snow"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceFinder_Grass(TEXT("/Game/0_MC/Materials/MI_Grass"));
	static ConstructorHelpers::FObjectFinder<UMaterialInstance> MaterialInstanceFinder_Stone(TEXT("/Game/0_MC/Materials/MI_Stone"));
	MaterialsMapping.Add(0, MaterialInstanceFinder_Snow.Object);
	MaterialsMapping.Add(1, MaterialInstanceFinder_Grass.Object);
	MaterialsMapping.Add(2, MaterialInstanceFinder_Stone.Object);

	static ConstructorHelpers::FObjectFinder<UStaticMesh> StaticMeshFinder(TEXT("/Game/0_MC/Mesh/SM_MCCube"));
	BoxMesh = StaticMeshFinder.Object;

	for (int32 i = 0; i < (MaterialsMapping.Num()); i++)
	{

		UInstancedStaticMeshComponent* NewComp = CreateDefaultSubobject<UInstancedStaticMeshComponent>(FName(FString("instancedMesh").Append(FString::FormatAsNumber(i))));
		if (NewComp)
		{
			//NewComp->AttachTo(GetRootComponent());
			// FAttachmentTransformRules SpawnTransRules = FAttachmentTransformRules::SnapToTargetIncludingScale;
			NewComp->SetupAttachment(GetRootComponent());

			InstancedBoxes.Add(NewComp);

			if (BoxMesh)
			{
				NewComp->SetStaticMesh(BoxMesh);

				if (MaterialsMapping.Contains(i))
				{
					NewComp->SetMaterial(0, MaterialsMapping.FindRef(i));
				}
			}
		}
	}
	// Create ISM for manually created blocks
	ManuallySpawnedGrass = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("ManuallySpawnedBlocks"));
		ManuallySpawnedGrass->SetupAttachment(GetRootComponent());
		InstancedBoxes.Add(ManuallySpawnedGrass);

	Area = 5;
	Depth = 3;
	VoxelSize = 100;
	NoiseDensity = 0.001;
	NoiseScale = 5;
	_3DNoiseDensity = 0.01;
	_3DNoiseCutOff = 0.f;
	SnowTreshold = 500;
	GrassTreshold = 350;

	// TODO: Remove from constructor and use only manually - BeginPlay?
	//SpawnWorldChunk();
}

// TODO
// Remove this chunk of code and request those data from PostActorCreated
void AMCWorldChunk::Init(class UStaticMesh* NewBoxMesh, int32 NewArea, int32 NewDepth, int32 NewVoxelSize, float NewNoiseDensity, int32 NewNoiseScale, float New3DNoiseDensity, float New3DNoiseCutOff)
{
	BoxMesh = NewBoxMesh;
	Area = NewArea;
	Depth = NewDepth;
	VoxelSize = NewVoxelSize;
	NoiseDensity = NewNoiseDensity;
	NoiseScale = NewNoiseScale;
	_3DNoiseDensity = New3DNoiseDensity;
	_3DNoiseCutOff = New3DNoiseCutOff;

	//SpawnWorldChunk();
}

void AMCWorldChunk::PostActorCreated()
{
	Super::PostActorCreated();
	
	// TODO
	// request values from AMCPlayerController to overwrite default ones

	if (GetOwner())
	{
		AMCPlayerController* OwningPlayerCon = Cast<AMCPlayerController>(GetWorld());
		if (OwningPlayerCon)
		{
			DeletedBlocksLocations.Empty();
			DeletedBlocksLocations = OwningPlayerCon->GetDeletedBlocksLocations();
		}
	}
}

void AMCWorldChunk::ForceNewInstance(FVector SpawnLoc, UInstancedStaticMeshComponent* SpawnISMC)
{
	if (SpawnISMC == nullptr) return;
	FTransform SpawnTransform = FTransform(FRotator(0), SpawnLoc, FVector(1));

	// TODO
	// Find material by index mapping and select right ISMC
	SpawnISMC->AddInstance(SpawnTransform);
}

void AMCWorldChunk::BeginPlay()
{
	Super::BeginPlay();
	
	SpawnWorldChunk();
}

void AMCWorldChunk::SpawnWorldChunk()
{
	// TODO: How can I optimize this part?
	for (int32 i = (Area * (-1)); i < Area; i++)
	{
		float RandomSeed = 0.f; //FMath::RandRange(0.0001f, 0.02f);

		LocalVoxelPos_X = i;

		for (int32 j = (Area * (-1)); j < Area; j++)
		{
			LocalVoxelPos_Y = j;

			for (int32 k = (Depth * (-1)); k < Depth; k++) // OPTIMISE <------- Do not create Depth blocks where not visible (detect whether location has cubes on all sides except bottom if so skip)
			{
				LocalVoxelPos_Z = k;

				FVector TempVector = (GetActorLocation() / VoxelSize);
				WorldVoxelPos_X = TempVector.X + LocalVoxelPos_X; // (FMath::FloorToInt(TempVector.X) + LocalVoxelPos_X);
				WorldVoxelPos_Y = TempVector.Y + LocalVoxelPos_Y; //(FMath::FloorToInt(TempVector.Y) + LocalVoxelPos_Y);
				WorldVoxelPos_Z_PreNoise = TempVector.Z + LocalVoxelPos_Z; //(FMath::FloorToInt(TempVector.Z) + LocalVoxelPos_Z);

				// TODO: clean this a bit
				// In code the noise is instanced and static therefore nonprocedural, in BP the noise is not instanced and procedural - why and how to fix it?
				// Standard UE PerlinNoise that can be used instead of SimplexNoise
				// FMath::PerlinNoise2D(FVector2D((VoxelSize * WorldVoxelPos_X), (VoxelSize * WorldVoxelPos_Y)));

				USimplexNoiseBPLibrary* SimplexLibrary = NewObject<USimplexNoiseBPLibrary>();
				
				if(!SimplexLibrary) return;

				

				float TempZf1 = SimplexLibrary->SimplexNoise2D((VoxelSize * WorldVoxelPos_X) + RandomSeed, (VoxelSize * WorldVoxelPos_Y) + RandomSeed, NoiseDensity + RandomSeed);
				float TempZf2 = (NoiseScale * TempZf1);
				int32 TempZi1 = FMath::FloorToInt(TempZf2);

				int32 TempZi2 = ((TempZi1 - LocalVoxelPos_Z) * VoxelSize) + RandomSeed;

				WorldVoxelPos_Z_Noised = TempZi2; //(FMath::FloorToInt(SimplexNoise2D((VoxelSize * WorldVoxelPos_X), (VoxelSize * WorldVoxelPos_Y), NoiseDensity)) - LocalVoxelPos_Z) * VoxelSize;

				if (LocalVoxelPos_Z * (-1) < (Depth - FMath::RandRange(0, 1)))
				{
					if (Get3DNoiseZ(_3DNoiseCutOff, _3DNoiseDensity))
					{
						SpawnCubeInstance();
					}
				}
				else
				{
					if (Get3DNoiseZ(-0.5, 0.025))
					{
						SpawnCubeInstance();
					}
				}
			}
		}
	}
}

bool AMCWorldChunk::Get3DNoiseZ(const float InNoiseCutof, const float InNoiseDensity)
{
	int RandomSeed = FMath::RandRange(-33, 26);

	USimplexNoiseBPLibrary* SimplexLibrary = NewObject<USimplexNoiseBPLibrary>(); // OPTIMISE <---------- move to inputs so I dont have to create new object every time

	if(!SimplexLibrary) 
	{
		return false;
	}
	return SimplexLibrary->SimplexNoise3D((VoxelSize * WorldVoxelPos_X) + RandomSeed, (VoxelSize * WorldVoxelPos_Y) + RandomSeed, (VoxelSize * WorldVoxelPos_Z_Noised) + RandomSeed, InNoiseCutof) <= NoiseDensity;
}

void AMCWorldChunk::SpawnCubeInstance()
{
	int32 activeIndex = 0;

	if(WorldVoxelPos_Z_Noised > SnowTreshold) 
	{
		activeIndex = 0;
	}
	else if (WorldVoxelPos_Z_Noised < GrassTreshold) 
	{
		activeIndex = 1;
	}
	else activeIndex = 2;
	/*else if ( here goes implementation of any other material type )
	{
		activeIndex = 2;
	}
	*/
	

	if(activeIndex < InstancedBoxes.Num()) // <------- active index must not be outside the range
	{
		AddVoxelInstanceOfType(InstancedBoxes[activeIndex]);
	}
}

bool AMCWorldChunk::IsBlockDeleted(FVector &CheckLocation) const
{
	if (DeletedBlocksLocations.Num() == 0) return false; // <----- no info means Im free to spawn everywhere

	bool bLocationDeleted = false;
	for (auto Itr : DeletedBlocksLocations)
	{	
		if (Itr == CheckLocation) // <----------- ask earlier that in the last iteration tho to save some performance?
		{
			bLocationDeleted = true;
			break;
		}
	}

	return bLocationDeleted;
}

void AMCWorldChunk::AddVoxelInstanceOfType(class UInstancedStaticMeshComponent* InstanceType)
{
	FVector SpawnLocation = FVector((VoxelSize * LocalVoxelPos_X), (VoxelSize * LocalVoxelPos_Y), (WorldVoxelPos_Z_Noised));
	FTransform InstanceSpawnTransform = FTransform(FRotator(0), SpawnLocation, FVector(1));
	
	if (IsBlockDeleted(SpawnLocation))
	{
		return;
	}

	if (InstanceType)
	{
		InstanceType->AddInstance(InstanceSpawnTransform);
	}
}