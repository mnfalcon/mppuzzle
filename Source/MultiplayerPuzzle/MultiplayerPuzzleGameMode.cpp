// Copyright Epic Games, Inc. All Rights Reserved.

#include "MultiplayerPuzzleGameMode.h"
#include "MultiplayerPuzzleCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "UI/ErrorNotificationDisplay.h"

AMultiplayerPuzzleGameMode::AMultiplayerPuzzleGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> ErrorDisplayBP(TEXT("/Game/UI/WBP_ErrorDisplay"));
	if (!ensure(ErrorDisplayBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("ErrorDisplayBP not found!"));
		return;
	}
	ErrorDisplayClass = ErrorDisplayBP.Class;
}

FString AMultiplayerPuzzleGameMode::InitNewPlayer(APlayerController* NewPlayerController, const FUniqueNetIdRepl& UniqueId, const FString& Options, const FString& Portal)
{
	FString showError = UGameplayStatics::ParseOption(Options, TEXT("showError"));
	FString errorTitle = UGameplayStatics::ParseOption(Options, TEXT("errorTitle"));
	FString errorMessage = UGameplayStatics::ParseOption(Options, TEXT("errorMessage"));
	UE_LOG(LogTemp, Warning, TEXT("OPTIONS: %s"), &Options);
	if (showError == "true") {
		if (ensure(ErrorDisplayClass != nullptr))
		{
			UErrorNotificationDisplay* ErrorWidget = CreateWidget<UErrorNotificationDisplay>(GetGameInstance(), ErrorDisplayClass);
			if (ErrorWidget != nullptr) {
				ErrorWidget->Setup(errorTitle, errorMessage);
				ErrorWidget->AddToViewport(1000);
			}
		}
	}
	return FString();
}
