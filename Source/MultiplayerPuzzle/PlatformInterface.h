// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMeshActor.h"
#include "PlatformInterface.generated.h"

UCLASS()
class MULTIPLAYERPUZZLE_API APlatformInterface : public AStaticMeshActor
{
	GENERATED_BODY()
	
private:

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Platform")
		int ActiveTriggers = 1;
	UPROPERTY(EditAnywhere, Category = "Platform")
		int RequiredTriggers = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Sets default values for this actor's properties
	APlatformInterface();

	void AddActiveTrigger() { ActiveTriggers++; }
	void RemoveActiveTrigger() { if(ActiveTriggers > 0) ActiveTriggers--; }
};
