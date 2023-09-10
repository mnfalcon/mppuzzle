// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MenuWidget.h"
#include "ErrorNotificationDisplay.generated.h"

/**
 * 
 */
UCLASS()

class MULTIPLAYERPUZZLE_API UErrorNotificationDisplay : public UMenuWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Body;

	UPROPERTY(meta = (BindWidget))
		class UTextBlock* Title;

	UPROPERTY(meta = (BindWidget))
		class UButton* CloseButton;

	void Setup(FString title, FString message);
	void CloseMessage();

protected:

	virtual bool Initialize() override;
	
};
