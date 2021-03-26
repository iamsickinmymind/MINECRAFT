// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MCPlane.generated.h"

UCLASS()
class MINECRAFT_API AMCPlane : public AActor
{
	GENERATED_BODY()

public:
	AMCPlane();

protected:
	virtual void BeginPlay() override;

	void SetPlaneSpawned(const bool NewValue);

public:
	virtual void Tick(float DeltaTime) override;

	FORCEINLINE bool IsPlaneSpawned() const { return bPlaneSpawned; };

	UFUNCTION(BlueprintCallable, CallInEditor)
	void SpawnCubes();

protected:
	// This mesh is used to generate terrain. Use simple block to avoid performance issues.
	UPROPERTY(VisibleAnywhere)
	UInstancedStaticMeshComponent* BlockPiece = nullptr;

	UPROPERTY(EditAnywhere, Category = "Plane Dimensions")
	int32 Lenght = 1;

	UPROPERTY(EditAnywhere, Category = "Plane Dimensions")
	int32 Width = 1;

	UPROPERTY(EditAnywhere, Category = "Plane Dimensions")
	int32 TileSize = 1;

private:

	UPROPERTY(VisibleAnywhere)
	bool bPlaneSpawned;
};
