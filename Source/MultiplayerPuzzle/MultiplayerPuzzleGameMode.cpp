// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerPuzzleGameMode.h"
#include "MultiplayerPuzzleCharacter.h"
#include "UObject/ConstructorHelpers.h"

AMultiplayerPuzzleGameMode::AMultiplayerPuzzleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
