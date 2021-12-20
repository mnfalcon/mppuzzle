// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "InGameMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API UInGameMenu : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonQuit;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBackToMain;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonCloseWidget;

	UFUNCTION()
		void QuitGame();
	UFUNCTION()
		void BackToMainMenu();

protected:

	virtual bool Initialize() override;

public:
	
	UFUNCTION()
		void OpenMenu();
	UFUNCTION()
		void CloseMenu();
};
