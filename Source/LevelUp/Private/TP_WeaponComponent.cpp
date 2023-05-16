// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_WeaponComponent.h"
#include "LevelUpCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"

// Sets default values for this component's properties
UTP_WeaponComponent::UTP_WeaponComponent()
{
	// Default offset from the character location for projectiles to spawn
	MuzzleOffset = FVector(100.0f, 0.0f, 10.0f);
}

void UTP_WeaponComponent::AttachWeapon(ALevelUpCharacter* TargetCharacter) 
{
	// Set up action bindings
	if (APlayerController* PlayerController = Cast<APlayerController>(TargetCharacter->GetController())) 
	{
		if (UEnhancedInputLocalPlayerSubsystem * Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer())) 
		{
			Subsystem->AddMappingContext(FireMappingContext, 1);
		}
	}
}

void UTP_WeaponComponent::Shoot(TSubclassOf<ALevelUpProjectile> ProjectileClass)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	check(OwnerPawn);

	if (!OwnerPawn->IsLocallyControlled())
	{
		return;
	}

	check(GetWorld());
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	check(GameState);

	FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
	SpawnParameters.Owner = GetOwner();
	SpawnParameters.Instigator = OwnerPawn;

	//calculating Target Transform
	const FRotator ControlRotation = SpawnParameters.Instigator->GetControlRotation();
	const FVector RightOffset = UKismetMathLibrary::GetRightVector(ControlRotation);
	const FVector UpOffset = UKismetMathLibrary::GetUpVector(ControlRotation);
	const FVector FrontOffset = FVector::CrossProduct(RightOffset, UpOffset) * MuzzleOffset.X;
	const FVector StartPos = FrontOffset + RightOffset * MuzzleOffset.Y + UpOffset * MuzzleOffset.Z + GetSocketLocation(TEXT("Muzzle"));
	const FVector Dir = SpawnParameters.Instigator->GetActorForwardVector();

	ALevelUpProjectile* Projectile = GetWorld()->SpawnActor<ALevelUpProjectile>(ProjectileClass, StartPos, Dir.Rotation(), SpawnParameters);

	FClientProjectileData ClientData;
	ClientData.Data = Projectile->GetData();
	ClientData.ProjectileClass = ProjectileClass;
	ClientData.StartPos = Projectile->GetActorLocation();
	ClientData.Dir = Dir;
	ClientData.ProjectilePtr = reinterpret_cast<int64>(Projectile);
	ClientData.MaxLifeTime = Projectile->InitialLifeSpan;
	ClientData.TimeStamp = GameState->GetServerWorldTimeSeconds();
	Server_Fire(ClientData);
}

void UTP_WeaponComponent::Client_DestroyFakeProjectile_Implementation(int64 ProjectilePtr)
{
	ALevelUpProjectile* Projectile = reinterpret_cast<ALevelUpProjectile*>(ProjectilePtr);
	if (IsValid(Projectile))
	{
		Projectile->Destroy();
	}
}

void UTP_WeaponComponent::Server_Fire_Implementation(const FClientProjectileData& ProjectileData)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	if (!IsValid(OwnerPawn))
	{
		return;
	}
	
	check(GetWorld());
	AGameStateBase* GameState = GetWorld()->GetGameState<AGameStateBase>();
	check(GameState);
	float LifeTime = GameState->GetServerWorldTimeSeconds() - ProjectileData.TimeStamp;
	FPredictProjectilePathParams Params(ProjectileData.Data.Radius, ProjectileData.StartPos, ProjectileData.Dir * ProjectileData.Data.LaunchVelocity, LifeTime);
	Params.bTraceWithCollision = true;
	Params.bTraceComplex = false;
	Params.ActorsToIgnore = {GetOwner()};
	Params.TraceChannel = ECollisionChannel::ECC_Visibility;
	FPredictProjectilePathResult PredictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, PredictResult);
	int32 PathDataNum = PredictResult.PathData.Num();
	FPredictProjectilePathPointData LastPathData(ProjectileData.StartPos, ProjectileData.Dir + ProjectileData.Data.LaunchVelocity, 0.0f);
	if (PathDataNum > 0)
	{
		LastPathData = PredictResult.PathData[PathDataNum - 1];
	}
	if (bHit)
	{
		ALevelUpProjectile::SimulateHit(ProjectileData.Data,
										OwnerPawn,
										PredictResult.HitResult.Location,
										LastPathData.Velocity,
										PredictResult.HitResult.GetActor(),
										PredictResult.HitResult.GetComponent());
	}
	else if (GetOwner()->GetRemoteRole() == ROLE_AutonomousProxy)
	{
		FActorSpawnParameters SpawnParameters = FActorSpawnParameters();
		SpawnParameters.Owner = GetOwner();
		SpawnParameters.Instigator = OwnerPawn;
		ALevelUpProjectile* Projectile = GetWorld()->SpawnActor<ALevelUpProjectile>(ProjectileData.ProjectileClass, 
																					ProjectileData.StartPos, 
																					ProjectileData.Dir.Rotation(), 
																					SpawnParameters);
		Projectile->SimulatePath(ProjectileData.TimeStamp);
		Client_DestroyFakeProjectile(ProjectileData.ProjectilePtr);
	}
}
