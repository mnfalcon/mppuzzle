// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"
#include "UI/InGameMenu.h"

const static FName SESSION_NAME = TEXT("Game Session 000");
UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Error, TEXT("Calling GameInstance constructor"));
	static ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTriggerBP(TEXT("/Game/BP_PlatformTrigger"));

	if (!ensure(PlatformTriggerBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("PlatformTriggerBPClass not found!"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *PlatformTriggerBP.Class->GetName());

	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("/Game/UI/WBP_MainMenu"));

	if (!ensure(MainMenuBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuBPClass not found!"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *MainMenuBP.Class->GetName());
	MainMenuClass = MainMenuBP.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBP(TEXT("/Game/UI/WBP_InGameMenu"));
	if (!ensure(InGameMenuBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("InGameMenuBP not found!"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *InGameMenuBP.Class->GetName());
	InGameMenuClass = InGameMenuBP.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	UE_LOG(LogTemp, Warning, TEXT("Calling GameInstance INIT"));
	//UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *MainMenuClass->GetName());
	//UE_LOG(LogTemp, Warning, TEXT("Found class %s"), *InGameMenuClass->GetName());

	IOnlineSubsystem* OSS = IOnlineSubsystem::Get();
	if (!OSS)
	{
		UE_LOG(LogTemp, Error, TEXT("OnlineSubsystem is null!"));
		return;
	}

	//UE_LOG(LogTemp, Warning, TEXT("OSS found! %s"), *OSS->GetSubsystemName().ToString());
	Session = OSS->GetSessionInterface();
	if (Session.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found session interface!"));
		Session->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
		Session->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
		Session->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);

		SessionSearch = MakeShareable(new FOnlineSessionSearch());
		if (SessionSearch.IsValid())
		{
			SessionSearch->bIsLanQuery = true;
			Session->FindSessions(0, SessionSearch.ToSharedRef());
		}
	}
	
}

void UPuzzlePlatformGameInstance::HostGame()
{
	if (Session.IsValid())
	{
		FNamedOnlineSession* ExistingSession = Session->GetNamedSession(SESSION_NAME);
		if (ExistingSession != nullptr)
		{
			Session->DestroySession(SESSION_NAME);
		}
		else
		{
			CreateSession();
		}
	}
}

void UPuzzlePlatformGameInstance::OnCreateSessionComplete(FName SessionName, bool Success)
{
	if (!Success)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to create session!"));
		return;
	}

	UE_LOG(LogTemp, Warning, TEXT("Session was created successfully"));
	if (Menu)
	{
		Menu->CloseMenu();
	}
	UEngine* Engine = GetEngine();

	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();

	if (!World) return;

	World->ServerTravel(TEXT("/Game/Maps/DebugPuzzle?listen"));
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession();
	}
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid())
	{
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *SessionSearch->SearchResults[i].GetSessionIdStr());
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No sessions found!"));
	}
}

void UPuzzlePlatformGameInstance::CreateSession()
{
	if (Session.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = true;
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;

		Session->CreateSession(0, SESSION_NAME, SessionSettings);
	}
}

void UPuzzlePlatformGameInstance::JoinGame(const FString& Address)
{
	if (Menu)
	{
		Menu->CloseMenu();
	}
	UEngine* Engine = GetEngine();

	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("Joining session %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();

	if (!PC)
	{
		//Engine->AddOnScreenDebugMessage(0, 3, FColor::Red, FString::Printf(TEXT("Failed joining session! Null PlayerController")));
		return;
	}
	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMenuWidget()
{
	if (MainMenuClass == nullptr) return;

	Menu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (Menu == nullptr) return;

	Menu->OpenMenu();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::OpenInGameMenu()
{
	if (InGameMenuClass == nullptr) return;

	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);

	if (InGameMenu == nullptr) return;

	InGameMenu->OpenMenu();
}
