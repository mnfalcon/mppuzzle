// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "OnlineSubsystem.h"
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

	IOnlineSessionPtr Session;

	TSharedPtr<class FOnlineSessionSearch> SessionSearch;

	void CreateSession();
	void OnCreateSessionComplete(FName SessionName, bool Success);
	void OnDestroySessionComplete(FName SessionName, bool Success);
	void OnFindSessionsComplete(bool Success);

public:
	virtual void Init() override;

	UPuzzlePlatformGameInstance(const FObjectInitializer& ObjectInitializer);

	UFUNCTION(exec)
		void HostGame() override; // overrides from IMenuInterface


	UFUNCTION(exec)
		void JoinGame(const FString& Address) override; // overrides from IMenuInterface

	UFUNCTION(exec, BlueprintCallable)
		void LoadMenuWidget();

	UFUNCTION(exec, BlueprintCallable)
		void OpenInGameMenu();
};
