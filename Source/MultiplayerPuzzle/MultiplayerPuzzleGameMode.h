// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MultiplayerPuzzleGameMode.generated.h"

UCLASS(minimalapi)
class AMultiplayerPuzzleGameMode : public AGameModeBase
{
	GENERATED_BODY()

private:
	TSubclassOf<class UUserWidget> ErrorDisplayClass;

public:
	AMultiplayerPuzzleGameMode();

protected:
	virtual FString InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal = TEXT("")) override;

};



