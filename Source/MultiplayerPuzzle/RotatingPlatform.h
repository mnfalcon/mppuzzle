// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "RotatingPlatform.generated.h"

/**
 * 
 */
UCLASS()
class MULTIPLAYERPUZZLE_API ARotatingPlatform : public AStaticMeshActor
{
	GENERATED_BODY()
private:
	UPROPERTY(EditAnywhere, Category = "RotatingPlatform")
		FRotator RotatorSpeed = FRotator(0, 20, 0);

	UPROPERTY(EditAnywhere, Category = "RotatingPlatform", meta = (AllowPrivateAccess = "true"))
		UStaticMeshComponent* PlatformMesh;

	USceneComponent* PivotPoint;

	UPROPERTY(VisibleAnywhere, Category = "RotatingPlatform", meta = (MakeEditWidget = "true"))
		FVector PivotPointLocation;
	UPROPERTY(EditAnywhere, Category = "RotatingPlatform", meta = (MakeEditWidget = "true", AllowPrivateAccess = "true"))
		FVector PlatformStartLocation;

protected:
	virtual void BeginPlay() override;

public:
	ARotatingPlatform();

	virtual void Tick(float DeltaSeconds) override;

	
};
