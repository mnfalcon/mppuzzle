// Fill out your copyright notice in the Description page of Project Settings.


#include "RotatingPlatform.h"

ARotatingPlatform::ARotatingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	PivotPoint = CreateDefaultSubobject<USceneComponent>(TEXT("Pivot Point"));
	SetRootComponent(PivotPoint);
	PlatformMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Platform"));
	PlatformMesh->SetupAttachment(RootComponent);
	SetMobility(EComponentMobility::Movable);
	SetActorHiddenInGame(false);
}

void ARotatingPlatform::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		SetReplicates(true);
		SetReplicateMovement(true);
	}

	PivotPointLocation = PivotPoint->GetComponentLocation();
	PlatformMesh->SetRelativeLocation(PlatformStartLocation);
}


void ARotatingPlatform::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (HasAuthority())
	{
		if (ActiveTriggers > RequiredTriggers)
		{
			AddActorWorldRotation(RotatorSpeed * DeltaSeconds);
		}
	}
}
