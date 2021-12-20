// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	UFUNCTION()
		void QuitGame();
public:
	UFUNCTION()
		virtual void OpenMenu();
	UFUNCTION()
		virtual void CloseMenu();
};
