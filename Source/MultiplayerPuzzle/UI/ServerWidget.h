// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ServerWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API UServerWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(meta = (BindWidget))
		class UTextBlock* ServerName;

	UPROPERTY(BlueprintReadOnly)
		bool Selected = false;

	void Setup(class UMainMenu* Parent, uint32 Index);
private:
	UPROPERTY(meta = (BindWidget))
		class UButton* RowButton;

	UPROPERTY()
	class UMainMenu* Parent;
	uint32 Index;

	UFUNCTION()
		void OnClicked();

	
};
