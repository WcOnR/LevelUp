// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameFramework/GameStateBase.h"
#include "TimerManager.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "Net/UnrealNetwork.h"

#include "AbilitySystemComponent.h"

ALevelUpProjectile::ALevelUpProjectile()
{
	PrimaryActorTick.TickGroup = TG_PostPhysics;
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
	SetReplicatingMovement(false);
	InitialLifeSpan = 3.0f;

	Data.Mass = 100.0f;
	Data.Radius = 5.0f;
	Data.LaunchVelocity = 3000.0f;
	BeginPlayTimeStamp = -1.0f;
}

void ALevelUpProjectile::SimulatePath(float CreateonTimeStamp)
{
	BeginPlayTimeStamp = CreateonTimeStamp;

	float LifeTimeLeft = InitialLifeSpan - (GameState->GetServerWorldTimeSeconds() - CreateonTimeStamp);
	FTimerHandle TimeHandle;
	GetWorldTimerManager().SetTimer(TimeHandle, this, &ALevelUpProjectile::OnLifeTimeEnd, LifeTimeLeft, false);
	PredictPath(LifeTimeLeft);
}

void ALevelUpProjectile::SimulateHit(const FProjectileData& SimData,
									 APawn* Instigator,
									 const FVector& Location,
									 const FVector& Velocity,
									 AActor* OtherActor,
									 UPrimitiveComponent* OtherComp)
{
	if (IsValid(Instigator) && OtherActor != nullptr && Instigator != OtherActor && OtherComp != nullptr)
	{
		if (Instigator->HasAuthority())
		{
			if (OtherComp->IsSimulatingPhysics())
			{
				OtherComp->AddImpulseAtLocation(Velocity * SimData.Mass, Location);
			}
			UAbilitySystemComponent* SourceASComponent = Instigator->FindComponentByClass<UAbilitySystemComponent>();
			UAbilitySystemComponent* TargetASComponent = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
			if (SourceASComponent && TargetASComponent)
			{
				FGameplayEffectContextHandle EffectContext = SourceASComponent->MakeEffectContext();
				EffectContext.AddSourceObject(Instigator);
				FGameplayEffectSpecHandle SpecHandle = SourceASComponent->MakeOutgoingSpec(SimData.DamageEffect, 1, EffectContext);
				SourceASComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASComponent);
			}
		}
	}
}

void ALevelUpProjectile::GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME_CONDITION(ALevelUpProjectile, TrueLocation, COND_None);
}

void ALevelUpProjectile::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!IsValid(GameState))
	{
		return;
	}
	if (HasAuthority())
	{
		const float LifeTime = GameState->GetServerWorldTimeSeconds() - BeginPlayTimeStamp;
		FPredictProjectilePathPointData PrevPathData = PathData[0];
		for (const FPredictProjectilePathPointData& PathPointData : PathData)
		{
			if (PathPointData.Time > LifeTime)
			{
				float Alpha = FMath::IsNearlyZero(LifeTime) ? 0.0f : (LifeTime - PrevPathData.Time) / (PathPointData.Time - PrevPathData.Time);
				SetActorLocation(FMath::Lerp(PrevPathData.Location, PathPointData.Location, Alpha));
				TrueLocation = GetActorLocation();
				RootComponent->ComponentVelocity = PathPointData.Velocity;
				break;
			}
			PrevPathData = PathPointData;
		}
		static const ETraceTypeQuery TraceType = UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility);
		FVector Loc = GetActorLocation();
		FHitResult OutHit;
		if (UKismetSystemLibrary::SphereTraceSingle(GetWorld(), Loc, Loc, Data.Radius, TraceType, false, {GetOwner()}, EDrawDebugTrace::None, OutHit, true))
		{
			HandleHit(OutHit.GetActor(), OutHit.GetComponent());
		}
	}
	else
	{
		ClientSinceUpdate += DeltaSeconds;
		float Alpha = !FMath::IsNearlyZero(ClientSinceLastUpdate) ? FMath::Min(ClientSinceUpdate / ClientSinceLastUpdate, 1.0f) : 1.0f;
		SetActorLocation(FMath::Lerp(ClientStartLocation, TrueLocation, Alpha));
	}
}

void ALevelUpProjectile::BeginPlay()
{
	Super::BeginPlay();
	check(GetWorld());
	GameState = GetWorld()->GetGameState<AGameStateBase>();
	check(GameState);
	
	if (IsValid(GetInstigator()) && GetInstigator()->IsLocallyControlled())
	{
		BeginPlayTimeStamp = GameState->GetServerWorldTimeSeconds();
		PredictPath(InitialLifeSpan);
	}
}

void ALevelUpProjectile::OnRep_TrueLocation()
{
	ClientStartLocation = GetActorLocation();
	ClientSinceLastUpdate = ClientSinceUpdate;
	ClientSinceUpdate = 0.0f;
}

void ALevelUpProjectile::PredictPath(float LifeTime)
{
	FPredictProjectilePathParams Params(Data.Radius, GetActorLocation(), GetActorForwardVector() * Data.LaunchVelocity, LifeTime);
	Params.bTraceWithCollision = false;
	FPredictProjectilePathResult PredictResult;
	UGameplayStatics::PredictProjectilePath(this, Params, PredictResult);
	PathData = PredictResult.PathData;
}

void ALevelUpProjectile::HandleHit(AActor* OtherActor, UPrimitiveComponent* OtherComp)
{
	APawn* Inst = GetInstigator();
	if (IsValid(Inst) && OtherActor != nullptr && OtherActor != this && Inst != OtherActor && OtherComp != nullptr)
	{
		SimulateHit(Data, Inst, GetActorLocation(), GetActorForwardVector() + Data.LaunchVelocity, OtherActor, OtherComp);
		Destroy();
	}
}

void ALevelUpProjectile::OnLifeTimeEnd()
{
	Destroy();
}
