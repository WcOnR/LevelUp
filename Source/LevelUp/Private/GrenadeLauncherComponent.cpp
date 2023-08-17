// Copyright Epic Games, Inc. All Rights Reserved.

#include "GrenadeLauncherComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Grenade.h"
#include "PathPredictComponent.h"

UGrenadeLauncherComponent::UGrenadeLauncherComponent()
{
}

void UGrenadeLauncherComponent::BeginPlay()
{
	Super::BeginPlay();
	PathPredict = Cast<UPathPredictComponent>(GetOwner()->FindComponentByClass(UPathPredictComponent::StaticClass()));
	FTimerHandle TimerHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &UGrenadeLauncherComponent::OnPositionUpdated, 0.05f, true);
}

void UGrenadeLauncherComponent::OnPositionUpdated()
{
	if (bShowPath)
	{
		AGrenade* GrenadeCDO = Cast<AGrenade>(GrenadeClass->GetDefaultObject(true));
		PreProjParams.StartLocation = GetComponentLocation();
		PreProjParams.LaunchVelocity = GetComponentRotation().Vector() * GrenadeCDO->GetMovementComponent()->InitialSpeed;
		FPredictProjectilePathResult PredictResult;
		UGameplayStatics::PredictProjectilePath(this, PreProjParams, PredictResult);
		PathPredict->DrawPath(PredictResult);
	}
}
void UGrenadeLauncherComponent::PredictPath()
{
	AGrenade* GrenadeCDO = Cast<AGrenade>(GrenadeClass->GetDefaultObject(true));
	UStaticMeshComponent* Mesh = GrenadeCDO->GetMeshComponent();
	UProjectileMovementComponent* Movement = GrenadeCDO->GetMovementComponent();
	FVector InitialSpeed = GetComponentRotation().Vector() * Movement->InitialSpeed;
	PreProjParams = FPredictProjectilePathParams(10.0f, GetComponentLocation(), InitialSpeed, GrenadeCDO->GetDelay());
	PreProjParams.bTraceWithCollision = true;
	PreProjParams.ActorsToIgnore.Add(GetOwner());
	bShowPath = true;
}

void UGrenadeLauncherComponent::ThrowGrenade()
{
	PathPredict->ClearPath();
	bShowPath = false;
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn);

	if (!OwnerPawn->IsLocallyControlled())
	{
		return;
	}
	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = OwnerPawn;
	GetWorld()->SpawnActor<AGrenade>(GrenadeClass, GetComponentLocation(), GetComponentRotation(), SpawnParameters);
}
