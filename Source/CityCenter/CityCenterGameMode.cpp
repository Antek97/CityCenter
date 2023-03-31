// Copyright Epic Games, Inc. All Rights Reserved.

#include "CityCenterGameMode.h"
#include "CityCenterHUD.h"
#include "CityCenterCharacter.h"
#include "UObject/ConstructorHelpers.h"

ACityCenterGameMode::ACityCenterGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = ACityCenterHUD::StaticClass();
}
