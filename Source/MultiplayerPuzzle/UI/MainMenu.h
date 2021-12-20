// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuInterface.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API UMainMenu : public UUserWidget
{
	GENERATED_BODY()
	
private:
		
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonHost;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonOpenJoinMenu;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonJoin;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBackToMain;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPInputBox;

	UFUNCTION()
		void HostGame();
	UFUNCTION()
		void JoinGame();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void BackToMainMenu();

	IMenuInterface* MenuInterface;

protected:

	virtual bool Initialize() override;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	void Setup();
	void Teardown();
};
