// Fill out your copyright notice in the Description page of Project Settings.


#include "PlatformTrigger.h"
#include "Components/BoxComponent.h"
#include "PlatformInterface.h"

// Sets default values
APlatformTrigger::APlatformTrigger()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Trigger Box"));

	if (!ensure(TriggerBox != nullptr))
	{
		return;
	}

	SetRootComponent(TriggerBox);
}

// Called when the game starts or when spawned
void APlatformTrigger::BeginPlay()
{
	Super::BeginPlay();
	
	if (TriggerBox)
	{
		TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapBegin);
		TriggerBox->OnComponentEndOverlap.AddDynamic(this, &APlatformTrigger::OnOverlapEnd);
	}
}

// Called every frame
void APlatformTrigger::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void APlatformTrigger::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("BeginOverlap"));

	if (!bInverseTriggerAddition)
	{
		for (int32 i = 0; i < Platforms.Num(); i++)
		{
			if (Platforms[i] != nullptr)
			{
				Platforms[i]->AddActiveTrigger();
			}
		}
	}
	else
	{
		for (int32 i = 0; i < Platforms.Num(); i++)
		{
			if (Platforms[i] != nullptr)
			{
				Platforms[i]->RemoveActiveTrigger();
			}
		}
	}
}

void APlatformTrigger::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("EndOverlap"));
	if (!bInverseTriggerAddition)
	{
		for (int32 i = 0; i < Platforms.Num(); i++)
		{
			if (Platforms[i] != nullptr)
			{
				Platforms[i]->RemoveActiveTrigger();
			}
		}
	}
	else
	{
		for (int32 i = 0; i < Platforms.Num(); i++)
		{
			if (Platforms[i] != nullptr)
			{
				Platforms[i]->AddActiveTrigger();
			}
		}
	}
}
