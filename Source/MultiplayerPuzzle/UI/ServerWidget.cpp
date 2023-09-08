// Fill out your copyright notice in the Description page of Project Settings.


#include "ServerWidget.h"
#include "MainMenu.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UServerWidget::Setup(UMainMenu* InParent, uint32 InIndex)
{
	Parent = InParent;
	Index = InIndex;
	if (IsHeader)
	{
		ServerName->SetJustification(ETextJustify::Center);
	}
	else {
		ServerName->SetJustification(ETextJustify::Left);
	}
	RowButton->OnClicked.AddDynamic(this, &UServerWidget::OnClicked);
}

void UServerWidget::OnClicked()
{
	Parent->SetSelectedIndex(Index);
}
