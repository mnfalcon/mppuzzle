// Fill out your copyright notice in the Description page of Project Settings.


#include "InGameMenu.h"
#include "Components/Button.h"
#include "../PuzzlePlatformGameInstance.h"

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

void UInGameMenu::BackToMainMenu()
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	PC->ClientTravel("/UI/MainMenu/", ETravelType::TRAVEL_Absolute);
}