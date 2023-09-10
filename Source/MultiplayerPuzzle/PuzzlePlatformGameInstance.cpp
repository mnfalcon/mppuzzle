// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "OnlineSessionSettings.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"
#include "UI/InGameMenu.h"
#include "UI/ErrorNotificationDisplay.h"


const static FName SESSION_NAME = NAME_GameSession;

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Warning, TEXT("Calling GameInstance constructor"));
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
	InGameMenuClass = InGameMenuBP.Class;
	/*UE_LOG(LogTemp, Warning, TEXT("Found %s"), *InGameMenuBP.Class->GetName());*/

	static ConstructorHelpers::FClassFinder<UUserWidget> ErrorDisplayBP(TEXT("/Game/UI/WBP_ErrorDisplay"));
	if (!ensure(ErrorDisplayBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("ErrorDisplayBP not found!"));
		return;
	}
	ErrorDisplayClass = ErrorDisplayBP.Class;
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

	UE_LOG(LogTemp, Warning, TEXT("OSS found! %s"), *OSS->GetSubsystemName().ToString());
	Session = OSS->GetSessionInterface();
	if (Session.IsValid())
	{
		//UE_LOG(LogTemp, Warning, TEXT("Found session interface"));
		Session->OnCreateSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnCreateSessionComplete);
		Session->OnDestroySessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnDestroySessionComplete);
		Session->OnFindSessionsCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnFindSessionsComplete);
		Session->OnJoinSessionCompleteDelegates.AddUObject(this, &UPuzzlePlatformGameInstance::OnJoinSessionComplete);
	}

	if (GEngine != nullptr)
	{
		GEngine->OnNetworkFailure().AddUObject(this, &UPuzzlePlatformGameInstance::OnNetworkFailure);
	}
	
}

void UPuzzlePlatformGameInstance::HostGame(const FString& serverName)
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
			CreateSession(serverName);
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

	World->ServerTravel(TEXT("/Game/Maps/Lobby?listen"));
}

void UPuzzlePlatformGameInstance::OnDestroySessionComplete(FName SessionName, bool Success)
{
	if (Success)
	{
		CreateSession(FString(""));
	}
}

void UPuzzlePlatformGameInstance::OnNetworkFailure(UWorld* World, UNetDriver* NetDriver, ENetworkFailure::Type FailureType, const FString& ErrorString)
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC != nullptr)) return;
	FString errorTitle = "Network Error";
	TMap<ENetworkFailure::Type, FString> NetworkFailureMap;

	// Populate the map with keys and values
	NetworkFailureMap.Add(ENetworkFailure::NetDriverAlreadyExists, TEXT("A relevant net driver has already been created for this service"));
	NetworkFailureMap.Add(ENetworkFailure::NetDriverCreateFailure, TEXT("The net driver creation failed"));
	NetworkFailureMap.Add(ENetworkFailure::NetDriverListenFailure, TEXT("The net driver failed its Listen() call"));
	NetworkFailureMap.Add(ENetworkFailure::ConnectionLost, TEXT("A connection to the net driver has been lost"));
	NetworkFailureMap.Add(ENetworkFailure::ConnectionTimeout, TEXT("A connection to the net driver has timed out"));
	NetworkFailureMap.Add(ENetworkFailure::FailureReceived, TEXT("The net driver received an NMT_Failure message"));
	NetworkFailureMap.Add(ENetworkFailure::OutdatedClient, TEXT("The client needs to upgrade their game"));
	NetworkFailureMap.Add(ENetworkFailure::OutdatedServer, TEXT("The server needs to upgrade their game"));
	NetworkFailureMap.Add(ENetworkFailure::PendingConnectionFailure, TEXT("There was an error during connection to the game"));
	NetworkFailureMap.Add(ENetworkFailure::NetGuidMismatch, TEXT("NetGuid mismatch"));
	NetworkFailureMap.Add(ENetworkFailure::NetChecksumMismatch, TEXT("Network checksum mismatch"));

	// Access values in the map by key
	FString ErrorMessage = NetworkFailureMap[FailureType];
	
	FString address = FString::Printf(TEXT("/Game/UI/MainMenu?showError=true?errorMessage=%s?errorTitle=%s"), *ErrorMessage, *errorTitle);
	PC->ClientTravel(address, ETravelType::TRAVEL_Absolute);

	UEngine* Engine = GetEngine();

	if (!Engine) return;
	Engine->AddOnScreenDebugMessage(0, 5, FColor::Red, TEXT("Network error. Returning to main menu."));
	UE_LOG(LogTemp, Warning, TEXT("Network error.Returning to main menu."));
}

void UPuzzlePlatformGameInstance::OnFindSessionsComplete(bool Success)
{
	if (Success && SessionSearch.IsValid() && Menu != nullptr)
	{
		TArray<FServerData> serverNames;
		for (int32 i = 0; i < SessionSearch->SearchResults.Num(); i++)
		{
			FOnlineSessionSearchResult& searchResult = SessionSearch->SearchResults[i];
			UE_LOG(LogTemp, Warning, TEXT("Found session: %s"), *SessionSearch->SearchResults[i].GetSessionIdStr());
			FServerData data;
			data.HostUsername = searchResult.Session.OwningUserName;
			data.MaxPlayers = searchResult.Session.SessionSettings.NumPublicConnections;
			data.CurrentPlayers = data.MaxPlayers - searchResult.Session.NumOpenPublicConnections;
			FString serverNameKey;
			FString serverName = searchResult.Session.SessionSettings.Get(TEXT("ServerName"), serverNameKey) ? serverNameKey : searchResult.GetSessionIdStr();
			data.Name = serverName;
			data.SessionId = searchResult.GetSessionIdStr();
			
			serverNames.Add(data);
		}
		Menu->SetServerList(serverNames);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("No sessions found!"));
	}
}

void UPuzzlePlatformGameInstance::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (!Session.IsValid()) return;
	
	FString address;
	if (!Session->GetResolvedConnectString(SessionName, address))
	{
		UE_LOG(LogTemp, Warning, TEXT("Could not get connection"));
		this->LogToScreen("Could not get connection", FColor::Red);
		return;
	}
	this->LogToScreen(FString::Printf(TEXT("Attempting to connect to %s"), *address), FColor::Red);
	UE_LOG(LogTemp, Warning, TEXT("JOINED SESSION %s with result %s"), *SessionName.ToString(), Result);
	this->JoinGame(address);
}

void UPuzzlePlatformGameInstance::LoadMainMenu()
{
	APlayerController* PC = GetFirstLocalPlayerController();
	if (!ensure(PC != nullptr)) return;
	PC->ClientTravel("/Game/UI/MainMenu", ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::CreateSession(const FString& serverName)
{
	if (Session.IsValid())
	{
		FOnlineSessionSettings SessionSettings;
		SessionSettings.bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL";
		SessionSettings.NumPublicConnections = 2;
		SessionSettings.bShouldAdvertise = true;
		SessionSettings.bUsesPresence = true;
		SessionSettings.Set(TEXT("ServerName"), serverName, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);

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

void UPuzzlePlatformGameInstance::LoadServers()
{
	SessionSearch = MakeShareable(new FOnlineSessionSearch());

	if (SessionSearch.IsValid() && Menu != nullptr)
	{
		//SessionSearch->bIsLanQuery = true;
		SessionSearch->MaxSearchResults = 100;
		SessionSearch->QuerySettings.Set(SEARCH_PRESENCE, true, EOnlineComparisonOp::Equals);
		Session->FindSessions(0, SessionSearch.ToSharedRef());
	}
}

void UPuzzlePlatformGameInstance::JoinGame(uint32 Index)
{
	if (!Session.IsValid()) return;

	if (!SessionSearch.IsValid()) return;

	if (Menu != nullptr)
	{
		Menu->CloseMenu();
	}
	Session->JoinSession(0, SESSION_NAME, SessionSearch->SearchResults[Index]);

}

void UPuzzlePlatformGameInstance::StartSession()
{
	if (Session.IsValid())
	{
		Session->StartSession(SESSION_NAME);
	}
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

void UPuzzlePlatformGameInstance::LogToScreen(FString message, FColor color)
{
	UEngine* Engine = GetEngine();

	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 3, color, message);
}
