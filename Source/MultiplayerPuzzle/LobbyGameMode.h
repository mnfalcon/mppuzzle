// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MultiplayerPuzzleGameMode.h"
#include "LobbyGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API ALobbyGameMode : public AMultiplayerPuzzleGameMode
{
	GENERATED_BODY()

public:
	void PostLogin(APlayerController* NewPlayer) override;
	void Logout(AController* Exiting) override;
	

private:

	void StartGame();
	uint16 PlayerCount = 0;
	FTimerHandle GameStartTimer;
	
};
