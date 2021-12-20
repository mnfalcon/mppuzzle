// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "../PuzzlePlatformGameInstance.h"
#include "Kismet/KismetSystemLibrary.h"

bool UInGameMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!ButtonCloseWidget) return false;
	ButtonCloseWidget->OnClicked.AddDynamic(this, &UInGameMenu::CloseMenu);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonCloseWidget->GetName());

	if (!ButtonQuit) return false;
	ButtonQuit->OnClicked.AddDynamic(this, &UInGameMenu::QuitGame);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonQuit->GetName());

	if (!ButtonBackToMain) return false;
	ButtonBackToMain->OnClicked.AddDynamic(this, &UInGameMenu::BackToMainMenu);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonBackToMain->GetName());

	return true;
}

void UInGameMenu::OpenMenu()
{
	this->AddToViewport();

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = World->GetFirstPlayerController();
	if (!PC) return;

	FInputModeUIOnly InputModeData;
	InputModeData.SetWidgetToFocus(this->TakeWidget());
	InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);

	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = true;

	UE_LOG(LogTemp, Warning, TEXT("Opening InGameMenu"));
}

void UInGameMenu::CloseMenu()
{
	this->RemoveFromViewport();

	UWorld* World = GetWorld();
	if (!World) return;

	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	FInputModeGameOnly InputModeData;
	PC->SetInputMode(InputModeData);
	PC->bShowMouseCursor = false;

}

void UInGameMenu::QuitGame()
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}

void UInGameMenu::BackToMainMenu()
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	PC->ClientTravel("/UI/MainMenu", ETravelType::TRAVEL_Absolute);
}