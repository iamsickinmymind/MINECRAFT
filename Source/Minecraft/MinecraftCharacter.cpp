// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/InputSettings.h"
#include <MCPlayerController.h>
//#include "Kismet/GameplayStatics.h"


DEFINE_LOG_CATEGORY_STATIC(LogFPChar, Warning, All);

AMinecraftCharacter::AMinecraftCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-39.56f, 1.75f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	Mesh1P->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
}

void AMinecraftCharacter::BeginPlay()
{
	Super::BeginPlay();
}

//////////////////////////////////////////////////////////////////////////
// Input

void AMinecraftCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// set up gameplay key bindings
	check(PlayerInputComponent);

	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	PlayerInputComponent->BindAxis("MoveForward", this, &AMinecraftCharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &AMinecraftCharacter::MoveRight);

	PlayerInputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &AMinecraftCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &AMinecraftCharacter::LookUpAtRate);

	PlayerInputComponent->BindAction("Escape", IE_Pressed, this, &AMinecraftCharacter::EscapePressed);

	PlayerInputComponent->BindAction("BuildingMode", IE_Pressed, this, &AMinecraftCharacter::BuildingRequest);
	PlayerInputComponent->BindAction("DiggingMode", IE_Pressed, this, &AMinecraftCharacter::DigRequest);
	PlayerInputComponent->BindAction("DiggingMode", IE_Released, this, &AMinecraftCharacter::DigRequest);
	PlayerInputComponent->BindAction("BuildingPressed", IE_Pressed, this, &AMinecraftCharacter::BuildingPressed);

	PlayerInputComponent->BindAction("Slot1", IE_Pressed, this, &AMinecraftCharacter::Slot1);
	PlayerInputComponent->BindAction("Slot2", IE_Pressed, this, &AMinecraftCharacter::Slot2);
	PlayerInputComponent->BindAction("Slot3", IE_Pressed, this, &AMinecraftCharacter::Slot3);
	PlayerInputComponent->BindAction("Slot4", IE_Pressed, this, &AMinecraftCharacter::Slot4);
}

void AMinecraftCharacter::EscapePressed()
{
	UE_LOG(LogTemp, Warning, TEXT("ESCAPE PRESSED - PLAYER"))

	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->EscapePressed();
	}
}

void AMinecraftCharacter::DigRequest()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->DigStarted();
	}
}

void AMinecraftCharacter::BuildingRequest()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->BuildingStarted();
	}
}

void AMinecraftCharacter::BuildingPressed()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->BuildingPressed();
	}
}

void AMinecraftCharacter::Slot1()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->SwitchBlockType(0);
	}
}

void AMinecraftCharacter::Slot2()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->SwitchBlockType(1);
	}
}

void AMinecraftCharacter::Slot3()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->SwitchBlockType(2);
	}
}

void AMinecraftCharacter::Slot4()
{
	if (AMCPlayerController* PlayerCon = Cast<AMCPlayerController>(Controller))
	{
		PlayerCon->SwitchBlockType(3);
	}
}

void AMinecraftCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void AMinecraftCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void AMinecraftCharacter::TurnAtRate(float Rate)
{
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AMinecraftCharacter::LookUpAtRate(float Rate)
{
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
