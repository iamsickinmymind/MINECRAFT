// Fill out your copyright notice in the Description page of Project Settings.


#include "MCPlayer.h"
#include "SimplexNoise/Public/SimplexNoiseBPLibrary.h"

// Sets default values
AMCPlayer::AMCPlayer()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMCPlayer::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AMCPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMCPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}