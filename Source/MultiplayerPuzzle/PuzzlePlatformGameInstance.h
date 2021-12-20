// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "UI/MenuInterface.h"
#include "PuzzlePlatformGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API UPuzzlePlatformGameInstance : public UGameInstance, public IMenuInterface
{
	GENERATED_BODY()
	
private:
	TSubclassOf<class UUserWidget> MainMenuClass;
	class UMainMenu* Menu;

	TSubclassOf<class UUserWidget> InGameMenuClass;
	class UInGameMenu* InGameMenu;
public:
	virtual void Init() override;

	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(exec)
		void HostGame() override; // overrides from IMenuInterface

	UFUNCTION(exec)
		void JoinGame(const FString& Address) override; // overrides from IMenuInterface

	UFUNCTION(exec, BlueprintCallable)
		void LoadMenu();

	UFUNCTION(exec, BlueprintCallable)
		void OpenInGameMenu();
};
