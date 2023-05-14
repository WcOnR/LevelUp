// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpProjectile.h"

#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"

ALevelUpProjectile::ALevelUpProjectile()
{
	// Use a sphere as a simple collision representation
	CollisionComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	CollisionComp->InitSphereRadius(5.0f);
	CollisionComp->BodyInstance.SetCollisionProfileName("Projectile");
	// set up a notification for when this component hits something blocking
	CollisionComp->OnComponentHit.AddDynamic(this, &ALevelUpProjectile::OnHit);

	// Players can't walk on it
	CollisionComp->SetWalkableSlopeOverride(FWalkableSlopeOverride(WalkableSlope_Unwalkable, 0.f));
	CollisionComp->CanCharacterStepUpOn = ECB_No;

	// Set as root component
	RootComponent = CollisionComp;

	// Use a ProjectileMovementComponent to govern this projectile's movement
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileComp"));
	ProjectileMovement->UpdatedComponent = CollisionComp;
	ProjectileMovement->InitialSpeed = 3000.f;
	ProjectileMovement->MaxSpeed = 3000.f;
	ProjectileMovement->bRotationFollowsVelocity = true;
	ProjectileMovement->bShouldBounce = true;

	// Die after 3 seconds by default
	InitialLifeSpan = 3.0f;

	Mass = 100.0f;
}

void ALevelUpProjectile::OnHit(
	UPrimitiveComponent* HitComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	// Only add impulse and destroy projectile if we hit a physics
	if (IsValid(InstigatedByPawn) && (OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr) && OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(GetVelocity() * Mass, GetActorLocation());
		if (UAbilitySystemComponent* SourceASComponent = InstigatedByPawn->FindComponentByClass<UAbilitySystemComponent>())
		{
			FGameplayEffectContextHandle EffectContext = SourceASComponent->MakeEffectContext();
			EffectContext.AddSourceObject(InstigatedByPawn);
			FGameplayEffectSpecHandle SpecHandle = SourceASComponent->MakeOutgoingSpec(DamageEffect, 1, EffectContext);
			SourceASComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), OtherActor->FindComponentByClass<UAbilitySystemComponent>());
		}
		Destroy();
	}
}