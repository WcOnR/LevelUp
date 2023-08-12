// Fill out your copyright notice in the Description page of Project Settings.


#include "MiniMap/Follower.h"

UFollower::UFollower()
{
	PrimaryComponentTick.bCanEverTick = false;
}


// Called when the game starts
void UFollower::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &UFollower::OnPositionUpdated, 0.05f, true);
}

void UFollower::OnPositionUpdated()
{
	if (AActor* Target = GetOwner()->GetOwner())
	{
		GetOwner()->SetActorLocation(Target->GetActorLocation(), false, nullptr, ETeleportType::TeleportPhysics);
	}
}

