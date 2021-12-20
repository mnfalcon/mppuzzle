// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuWidget.h"
#include "Kismet/KismetSystemLibrary.h"

void UMenuWidget::OpenMenu()
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

void UMenuWidget::CloseMenu()
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

void UMenuWidget::QuitGame()
{
	APlayerController* PC = GetGameInstance()->GetFirstLocalPlayerController();
	if (!PC) return;

	PC->ConsoleCommand("quit");
	//UKismetSystemLibrary::QuitGame(GetWorld(), PC, EQuitPreference::Quit, true);
}