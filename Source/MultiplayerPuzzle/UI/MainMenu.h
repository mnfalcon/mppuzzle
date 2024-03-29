// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuInterface.h"
#include "MenuWidget.h"
#include "MainMenu.generated.h"

USTRUCT()
struct FServerData 
{
	GENERATED_BODY()

	FString Name;
	uint16 CurrentPlayers;
	uint16 MaxPlayers;
	FString HostUsername;
	FString SessionId;
};

/**
 * 
 */

UCLASS()
class MULTIPLAYERPUZZLE_API UMainMenu : public UMenuWidget
{
	GENERATED_BODY()
	
private:

	bool bIsHosting = false;
	bool bIsInServerListMenu = false;

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

	UPROPERTY(meta = (BindWidget), EditAnywhere)
		UButton* RefreshServerListButton;

	UPROPERTY(meta = (BindWidget))
		class UWidgetSwitcher* MenuSwitcher;

	UPROPERTY(meta = (BindWidget))
		class UWidget* JoinMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* MainMenu;

	UPROPERTY(meta = (BindWidget))
		UWidget* HostGameMenu;

	UPROPERTY(meta = (BindWidget))
		UButton* HostGameMenuButtonBackToMain;

	UPROPERTY(meta = (BindWidget))
		UButton* ButtonCreateSession;

	UPROPERTY(meta = (BindWidget))
		class UEditableTextBox* ServerNameInputBox;

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
	void DisplayNotification(FString title, FString message);
	UFUNCTION()
		void RefreshServerList();
	UFUNCTION()
		void OpenHostGameMenu();

	void UpdateChildren();

	IMenuInterface* MenuInterface;

	TOptional<uint32> SelectedIndex;

protected:

	virtual bool Initialize() override;

public:
	void SetMenuInterface(IMenuInterface* MenuInterface);

	UMainMenu(const FObjectInitializer& ObjectInitializer);

	void SetServerList(TArray<FServerData> servers);

	void SetSelectedIndex(uint32 index);

	UPROPERTY(BlueprintReadOnly)
		bool isLoadingServers = true;
};
