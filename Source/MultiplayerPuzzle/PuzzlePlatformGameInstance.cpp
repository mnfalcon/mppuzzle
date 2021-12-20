// Fill out your copyright notice in the Description page of Project Settings.


#include "PuzzlePlatformGameInstance.h"
#include "Engine/Engine.h"
#include "UObject/ConstructorHelpers.h"
#include "PlatformTrigger.h"
#include "Blueprint/UserWidget.h"
#include "UI/MainMenu.h"
#include "UI/InGameMenu.h"

UPuzzlePlatformGameInstance::UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer)
{
	UE_LOG(LogTemp, Error, TEXT("Calling GameInstance constructor"));
	static ConstructorHelpers::FClassFinder<APlatformTrigger> PlatformTriggerBP(TEXT("/Game/BP_PlatformTrigger"));

	if (!ensure(PlatformTriggerBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("PlatformTriggerBPClass not found!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *PlatformTriggerBP.Class->GetName());

	static ConstructorHelpers::FClassFinder<UUserWidget> MainMenuBP(TEXT("/Game/UI/WBP_MainMenu"));

	if (!ensure(MainMenuBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuBPClass not found!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *MainMenuBP.Class->GetName());
	MainMenuClass = MainMenuBP.Class;

	static ConstructorHelpers::FClassFinder<UUserWidget> InGameMenuBP(TEXT("/Game/UI/WBP_InGameMenu"));
	if (!ensure(InGameMenuBP.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("InGameMenuBP not found!"));
		return;
	}
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *InGameMenuBP.Class->GetName());
	InGameMenuClass = InGameMenuBP.Class;
}

void UPuzzlePlatformGameInstance::Init()
{
	UE_LOG(LogTemp, Error, TEXT("Calling GameInstance INIT"));
	UE_LOG(LogTemp, Error, TEXT("Found class %s"), *MainMenuClass->GetName());
	UE_LOG(LogTemp, Error, TEXT("Found class %s"), *InGameMenuClass->GetName());
}

void UPuzzlePlatformGameInstance::HostGame()
{
	if (Menu)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();

	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, TEXT("Hosting"));

	UWorld* World = GetWorld();

	if (!World) return;

	World->ServerTravel("/Game/Maps/DebugPuzzle?listen");
}

void UPuzzlePlatformGameInstance::JoinGame(const FString& Address)
{
	if (Menu)
	{
		Menu->Teardown();
	}
	UEngine* Engine = GetEngine();

	if (!Engine) return;

	Engine->AddOnScreenDebugMessage(0, 3, FColor::Green, FString::Printf(TEXT("Joining session %s"), *Address));

	APlayerController* PC = GetFirstLocalPlayerController();

	if (!PC)
	{
		Engine->AddOnScreenDebugMessage(0, 3, FColor::Red, FString::Printf(TEXT("Failed joining session! Null PlayerController")));
		return;
	}
	PC->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
}

void UPuzzlePlatformGameInstance::LoadMenu()
{
	if (MainMenuClass == nullptr) return;

	Menu = CreateWidget<UMainMenu>(this, MainMenuClass);

	if (Menu == nullptr) return;

	Menu->Setup();
	Menu->SetMenuInterface(this);
}

void UPuzzlePlatformGameInstance::OpenInGameMenu()
{
	if (InGameMenuClass == nullptr) return;

	InGameMenu = CreateWidget<UInGameMenu>(this, InGameMenuClass);

	if (InGameMenu == nullptr) return;

	InGameMenu->OpenMenu();
}

void UPuzzlePlatformGameInstance::CloseInGameMenu()
{
	if (InGameMenuClass == nullptr) return;

	if (InGameMenu == nullptr) return;

	InGameMenu->CloseMenu();
}