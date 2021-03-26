// Copyright Epic Games, Inc. All Rights Reserved.

#include "MinecraftGameMode.h"
#include "MinecraftHUD.h"
#include "MinecraftCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "MCPlayerController.h"

AMinecraftGameMode::AMinecraftGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<AMinecraftCharacter> PlayerPawnClassFinder(TEXT("/Game/0_MC/Blueprints/Core/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	static ConstructorHelpers::FClassFinder<AMCPlayerController> PlayerControllerClassFinder(TEXT("/Game/0_MC/Blueprints/Core/BP_PlayerController"));
	PlayerControllerClass = PlayerControllerClassFinder.Class;

	// use our custom HUD class
	HUDClass = AMinecraftHUD::StaticClass();
}
