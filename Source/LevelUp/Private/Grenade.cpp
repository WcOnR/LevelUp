// Copyright Epic Games, Inc. All Rights Reserved.

#include "Grenade.h"

#include "GameplayEffect.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

AGrenade::AGrenade()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	SetRootComponent(Mesh);

	DamageSphere = CreateDefaultSubobject<USphereComponent>(TEXT("DamageSphere"));
	DamageSphere->SetupAttachment(Mesh);

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	ExplosionImpulse = 50000.0f;
}

void AGrenade::BeginPlay()
{
	Super::BeginPlay();
	FTimerHandle TimerHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &AGrenade::OnDelayEnd, Delay, false);
}

void AGrenade::OnDelayEnd()
{
	const FVector Loc = GetActorLocation();
	UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Fx, Loc);
	
	UAbilitySystemComponent* SourceASComponent = GetOwner()->FindComponentByClass<UAbilitySystemComponent>();
	TArray<AActor*> OverlappingActors;
	DamageSphere->GetOverlappingActors(OverlappingActors);
	for (AActor* Actor : OverlappingActors)
	{
		UPrimitiveComponent* OtherComp = Actor->FindComponentByClass<UPrimitiveComponent>();
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			FVector Dir = (OtherComp->GetComponentLocation() - Loc).GetUnsafeNormal();
			OtherComp->AddImpulse(ExplosionImpulse * Dir);
		}
		
		UAbilitySystemComponent* TargetASComponent = Actor->FindComponentByClass<UAbilitySystemComponent>();
		if (SourceASComponent && TargetASComponent)
		{
			FGameplayEffectContextHandle EffectContext = SourceASComponent->MakeEffectContext();
			EffectContext.AddSourceObject(GetOwner());
			FGameplayEffectSpecHandle SpecHandle = SourceASComponent->MakeOutgoingSpec(DamageEffect, 1, EffectContext);
			SourceASComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASComponent);
		}
	}

	Destroy();
}
