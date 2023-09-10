// Fill out your copyright notice in the Description page of Project Settings.


#include "ErrorNotificationDisplay.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"

void UErrorNotificationDisplay::Setup(FString title, FString message)
{
	Title->SetText(FText::FromString(title));
	Body->SetText(FText::FromString(message));
}

void UErrorNotificationDisplay::CloseMessage()
{
	
}

bool UErrorNotificationDisplay::Initialize()
{
	if (!Super::Initialize()) return false;
	if (!CloseButton) return false;
	CloseButton->OnClicked.AddDynamic(this, &UErrorNotificationDisplay::RemoveFromViewport);
	return true;
}


