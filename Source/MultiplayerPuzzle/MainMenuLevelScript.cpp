// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenuLevelScript.h"
#include "PuzzlePlatformGameInstance.h"

void AMainMenuLevelScript::BeginPlay()
{
	auto GameInstance = Cast<UPuzzlePlatformGameInstance>(GetGameInstance());
	if (GameInstance == nullptr) return;

	GameInstance->LoadMenuWidget();
}

