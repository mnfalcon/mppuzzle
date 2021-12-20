// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlatformInterface.h"
#include "MovingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API AMovingPlatform : public APlatformInterface
{
	GENERATED_BODY()
	
private:

	UPROPERTY(EditAnywhere, Category = "MovingPlatform", meta = (AllowPrivateAccess = "true", MakeEditWidget = "true"))
		FVector TargetLocation;
	UPROPERTY(EditAnywhere, Category = "MovingPlatform")
		float Speed = 1.f;

	FVector GlobalTargetLocation;
	FVector GlobalStartLocation;

protected:
	virtual void BeginPlay() override;
public:
	AMovingPlatform();

	virtual void Tick(float DeltaSeconds) override;
};
