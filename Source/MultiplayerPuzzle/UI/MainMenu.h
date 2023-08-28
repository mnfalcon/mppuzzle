// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
private:

	bool bIsHosting = false;

	TSubclassOf<class UUserWidget> ServerRowClass;
		
	UPROPERTY(meta = (BindWidget))
		class UButton* ButtonHost;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonOpenJoinMenu;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonOpenServerList;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonJoin;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonBackToMain;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonJoinServer;

	UPROPERTY(meta = (BindWidget))
		UButton* ServerScreenButtonBackToMain;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonQuitGame;

	UPROPERTY(meta = (BindWidget))
		UButton* RefreshServerListButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* IPInputBox;

	UPROPERTY(meta = (BindWidget))
		class UWidget* ServerListMenu;

	UPROPERTY(meta = (BindWidget))
		class UPanelWidget* ServerListBox;

	UFUNCTION()
		void HostGame();
	UFUNCTION()
		void JoinGame();
	UFUNCTION()
		void OpenServerListScreen();
	UFUNCTION()
		void JoinServer();
	UFUNCTION()
		void OpenJoinMenu();
	UFUNCTION()
		void BackToMainMenu();
	UFUNCTION()
		void RefreshServerList();

	IMenuInterface* MenuInterface;

protected:

	virtual bool Initialize() override;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FString> serverNames);
};
