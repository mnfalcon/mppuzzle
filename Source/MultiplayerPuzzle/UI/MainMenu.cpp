// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "../PuzzlePlatformGameInstance.h"

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!ButtonHost) return false;
	ButtonHost->OnClicked.AddDynamic(this, &UMainMenu::HostGame);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonHost->GetName());

	if (!ButtonOpenJoinMenu) return false;
	ButtonOpenJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonOpenJoinMenu->GetName());

	if (!ButtonBackToMain) return false;
	ButtonBackToMain->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonBackToMain->GetName());

	if (!ButtonJoin) return false;
	ButtonJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinGame);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonJoin->GetName());

	if (!ButtonQuitGame) return false;
	ButtonQuitGame->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonQuitGame->GetName());

	return true;
}

void UMainMenu::SetMenuInterface(IMenuInterface* MenuInterfaceImplementation)
{
	this->MenuInterface = MenuInterfaceImplementation;
}

void UMainMenu::HostGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Hosting game"));

	if (MenuInterface)
	{
		MenuInterface->HostGame();
	}
}

void UMainMenu::JoinGame()
{
	UE_LOG(LogTemp, Warning, TEXT("Joining game"));

	if (!IPInputBox) return;
	if (!MenuInterface) return;

	const FString& IpAddress = IPInputBox->GetText().ToString();
	MenuInterface->JoinGame(IpAddress);
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	UE_LOG(LogTemp, Warning, TEXT("Opening Join Menu"));
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::BackToMainMenu()
{
	if (!MenuSwitcher) return;
	if (!MainMenu) return;
	UE_LOG(LogTemp, Warning, TEXT("Opening Join Menu"));

	MenuSwitcher->SetActiveWidget(MainMenu);
}

