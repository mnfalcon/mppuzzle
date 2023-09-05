// Fill out your copyright notice in the Description page of Project Settings.


#include "MainMenu.h"
#include "Components/Button.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetSwitcher.h"
#include "Components/EditableTextBox.h"
#include "../PuzzlePlatformGameInstance.h"
#include "Components/TextBlock.h"
#include "ServerWidget.h"

UMainMenu::UMainMenu(const FObjectInitializer& ObjectInitializer)
{
	static ConstructorHelpers::FClassFinder<UUserWidget> ServerRowBPClass(TEXT("/Game/UI/WBP_ServerRow"));

	if (!ensure(ServerRowBPClass.Class != nullptr))
	{
		UE_LOG(LogTemp, Error, TEXT("MainMenuBPClass not found!"));
		return;
	}
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *MainMenuBP.Class->GetName());
	ServerRowClass = ServerRowBPClass.Class;
}

bool UMainMenu::Initialize()
{
	if (!Super::Initialize()) return false;

	if (!ButtonHost) return false;
	ButtonHost->OnClicked.AddDynamic(this, &UMainMenu::HostGame);
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonHost->GetName());

	if (!ButtonOpenJoinMenu) return false;
	ButtonOpenJoinMenu->OnClicked.AddDynamic(this, &UMainMenu::OpenJoinMenu);
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonOpenJoinMenu->GetName());

	if (!ButtonBackToMain) return false;
	ButtonBackToMain->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);

	if (!ServerScreenButtonBackToMain) return false;
	ServerScreenButtonBackToMain->OnClicked.AddDynamic(this, &UMainMenu::BackToMainMenu);
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonBackToMain->GetName());

	if (!ButtonJoin) return false;
	ButtonJoin->OnClicked.AddDynamic(this, &UMainMenu::JoinGame);
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonJoin->GetName());

	if (!ButtonQuitGame) return false;
	ButtonQuitGame->OnClicked.AddDynamic(this, &UMainMenu::QuitGame);
	//UE_LOG(LogTemp, Warning, TEXT("Found %s"), *ButtonQuitGame->GetName());

	if (!ButtonOpenServerList) return false;
	ButtonOpenServerList->OnClicked.AddDynamic(this, &UMainMenu::OpenServerListScreen);

	if (!ButtonJoinServer) return false;
	ButtonJoinServer->OnClicked.AddDynamic(this, &UMainMenu::JoinServer);

	if (!RefreshServerListButton) return false;
	RefreshServerListButton->OnClicked.AddDynamic(this, &UMainMenu::RefreshServerList);

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
		bIsHosting = true;
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

void UMainMenu::OpenServerListScreen()
{
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	UE_LOG(LogTemp, Warning, TEXT("Opening Server List"));
	MenuSwitcher->SetActiveWidget(ServerListMenu);
	if (!MenuInterface) return;
	MenuInterface->LoadServers();
}

void UMainMenu::JoinServer()
{
	if (MenuInterface != nullptr && SelectedIndex.IsSet())
	{
		UE_LOG(LogTemp, Warning, TEXT("Joining server"));
		MenuInterface->JoinGame(SelectedIndex.GetValue());
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Selected index not set"));
	}
}

void UMainMenu::SetServerList(TArray<FString> serverNames)
{
	if (bIsHosting) return;

	UWorld* World = this->GetWorld();
	if (!ensure(World != nullptr)) return;

	ServerListBox->ClearChildren();

	// TODO add functionality to refresh button in the UI
	uint32 i = 0;
	for (const FString& serverName : serverNames)
	{
		UServerWidget* Row = CreateWidget<UServerWidget>(World, ServerRowClass);
		Row->ServerName->SetText(FText::FromString(serverName));
		Row->Setup(this, i++);
		ServerListBox->AddChild(Row);
	}
}

void UMainMenu::SetSelectedIndex(uint32 index)
{
	SelectedIndex = index;
	UpdateChildren();
}

void UMainMenu::RefreshServerList()
{
	if (!MenuInterface) return;
	UE_LOG(LogTemp, Warning, TEXT("Refreshing server list"));
	MenuInterface->LoadServers();
}

void UMainMenu::UpdateChildren()
{
	for (int32 i = 0; i < ServerListBox->GetChildrenCount(); i++)
	{
		UServerWidget* row = Cast<UServerWidget>(ServerListBox->GetChildAt(i));
		if (row != nullptr)
		{
			row->Selected = (SelectedIndex.IsSet() && i == SelectedIndex.GetValue());
		}
	}
}

void UMainMenu::OpenJoinMenu()
{
	if (!MenuSwitcher) return;
	if (!JoinMenu) return;
	//UE_LOG(LogTemp, Warning, TEXT("Opening Join Menu"));
	MenuSwitcher->SetActiveWidget(JoinMenu);
}

void UMainMenu::BackToMainMenu()
{
	if (!MenuSwitcher) return;
	if (!MainMenu) return;
	//UE_LOG(LogTemp, Warning, TEXT("Opening Join Menu"));

	MenuSwitcher->SetActiveWidget(MainMenu);
	bIsHosting = false;
}

