// Fill out your copyright notice in the Description page of Project Settings.


#include "LobbyGameMode.h"
#include "TimerManager.h"
#include "PuzzlePlatformGameInstance.h"

void ALobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	Super::PostLogin(NewPlayer);
	++PlayerCount;
	UE_LOG(LogTemp, Warning, TEXT("A new player joins the adventure! Current player count: %d"), PlayerCount);

	if (PlayerCount >= 2)
	{
		UE_LOG(LogTemp, Warning, TEXT("We are ready to start the adventure! Current player count: %d"), PlayerCount);
		GetWorldTimerManager().SetTimer(GameStartTimer, this, &ALobbyGameMode::StartGame, 5.f);
	}
}

void ALobbyGameMode::Logout(AController* Exiting)
{
	Super::Logout(Exiting);
	if (PlayerCount != 0)
	{
		--PlayerCount;
	}
	UE_LOG(LogTemp, Warning, TEXT("Oh no! A player has abandoned us! Current player count: %d"), PlayerCount);
}

void ALobbyGameMode::StartGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Starting the adventure!"));

	auto GameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	GameInstance->StartSession();

	UWorld* World = GetWorld();
	if (!ensure(World != nullptr)) return;
	bUseSeamlessTravel = true;
	World->ServerTravel("/Game/Maps/DebugPuzzle?listen");
}
