// Copyright Epic Games, Inc. All Rights Reserved.

#include "Module5ProjGameMode.h"
#include "Module5ProjHUD.h"
#include "Module5ProjCharacter.h"
#include "UObject/ConstructorHelpers.h"

AModule5ProjGameMode::AModule5ProjGameMode()
	: Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPersonCPP/Blueprints/FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AModule5ProjHUD::StaticClass();
}
