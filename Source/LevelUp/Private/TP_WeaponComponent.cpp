// Copyright Epic Games, Inc. All Rights Reserved.

#include "TP_WeaponComponent.h"
#include "LevelUpCharacter.h"
#include "Camera/CameraComponent.h"
#include "Engine/LocalPlayer.h"
#include "Engine/NetSerialization.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerController.h"
#include "GameplayEffect.h"
#include "AbilitySystemComponent.h"
#include "NiagaraSystem.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "WeaponData.h"
#include "DamageReceiverInterface.h"

static const FName MuzzleSlot(TEXT("Muzzle"));
static const FName EndPointParam(TEXT("EndPoint"));
static const FName ImpactParam(TEXT("IsImpactEnabled"));

bool FLaunchRay::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	bOutSuccess = SerializePackedVector<1, 20>(Origin, Ar);
	bOutSuccess &= SerializeFixedVector<1, 16>(Dir, Ar);
	return true;
}

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

void UTP_WeaponComponent::Shoot(ULevelUpGameplayAbility* Ability)
{
	FWeaponData Data;
	int32 SlotId = FindWeaponData(Ability, Data);
	if (SlotId == -1)
	{
		return;
	}
	if (IsValid(Data.ProjectileClass.Get()))
	{
		ThrowProjectile(Data.ProjectileClass.Get(), Data.DamageEffect.Get());
	}
	else if (IsValid(Data.RayTraceFx.Get()))
	{
		FHitResult OutHit;
		bool bHitted = GetTargetHit(OutHit, Data.MaxDistance);
		RayTraceData.SlotId = SlotId;
		RayTraceData.Fx = UNiagaraFunctionLibrary::SpawnSystemAttached(Data.RayTraceFx.Get(), this, MuzzleSlot,
															 FVector::Zero(), GetTargetDirection(OutHit.Location).Rotation(),
															 EAttachLocation::SnapToTarget, false);
	}
}

void UTP_WeaponComponent::StopShoot(ULevelUpGameplayAbility* Ability)
{
	if (IsValid(RayTraceData.Fx))
	{
		RayTraceData.Fx->DestroyComponent();
		RayTraceData.Fx = nullptr;
	}
}

void UTP_WeaponComponent::ThrowProjectile(TSubclassOf<ALevelUpProjectile> ProjClass, TSubclassOf<UGameplayEffect> DamageEffect)
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

	ALevelUpProjectile* Projectile = GetWorld()->SpawnActor<ALevelUpProjectile>(ProjClass, StartPos, Dir.Rotation(), SpawnParameters);

	FClientProjectileData ClientData;
	ClientData.Data = Projectile->GetData();
	ClientData.Data.DamageEffect = DamageEffect;
	ClientData.ProjectileClass = ProjClass;
	ClientData.LaunchRay = FLaunchRay(Projectile->GetActorLocation(), Dir);
	ClientData.ProjectilePtr = reinterpret_cast<int64>(Projectile);
	ClientData.MaxLifeTime = Projectile->InitialLifeSpan;
	ClientData.TimeStamp = GameState->GetServerWorldTimeSeconds();
	Server_ThrowProjectile(ClientData);
}

void UTP_WeaponComponent::BeginPlay()
{
	Super::BeginPlay();
	LoadWeaponData();
	if (ALevelUpCharacter* Char = Cast<ALevelUpCharacter>(GetOwner()))
	{

		TArray<TSubclassOf<ULevelUpGameplayAbility>> Abilities;
		for (const FWeaponData& Slot : WeaponSlotsData)
		{
			Abilities.Add(Slot.Ability.Get());
		}
		Char->AddWeaponAbilities(Abilities);
	}

	FTimerHandle TimerHandle;
	GetOwner()->GetWorldTimerManager().SetTimer(TimerHandle, this, &UTP_WeaponComponent::OnPositionUpdated, 0.05f, true);
}

void UTP_WeaponComponent::OnPositionUpdated()
{
	if (!IsValid(RayTraceData.Fx))
	{
		return;
	}
	FHitResult OutHit;
	const FWeaponData& WeaponData = WeaponSlotsData[RayTraceData.SlotId];
	bool bHitted = GetTargetHit(OutHit, WeaponData.MaxDistance);
	RayTraceData.Fx->SetRelativeRotation(GetTargetDirection(OutHit.Location).Rotation());
	RayTraceData.Fx->SetVectorParameter(EndPointParam, FVector(FVector::Distance(OutHit.Location, GetSocketLocation(MuzzleSlot)), 0.0f, 0.0f));
	RayTraceData.Fx->SetIntParameter(ImpactParam, bHitted);
	if (bHitted)
	{
		ApplyHitEffect(OutHit, WeaponData.DamageEffect.Get(), WeaponData.Impact);
	}
}

bool UTP_WeaponComponent::GetTargetHit(FHitResult& OutHit, float MaxDistance)
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());
	UCameraComponent* Camera = OwnerPawn->FindComponentByClass<UCameraComponent>();

	FVector End = Camera->GetComponentLocation() + Camera->GetForwardVector() * MaxDistance;

	FCollisionQueryParams CollisionParams;
	if (GetWorld()->LineTraceSingleByChannel(OutHit, Camera->GetComponentLocation(), End, ECC_Visibility, CollisionParams))
	{
		return true;
	}
	else
	{
		OutHit.Location = End;
	}
	return false;
}

FVector UTP_WeaponComponent::GetTargetDirection(const FVector& Hit)
{
	const FTransform DirTransform((Hit - GetSocketLocation(MuzzleSlot)).GetUnsafeNormal());
	const FTransform MuzzleTransform(GetSocketRotation(MuzzleSlot));
	return (DirTransform * MuzzleTransform.Inverse()).GetLocation();
}

void UTP_WeaponComponent::ApplyHitEffect(const FHitResult& OutHit, TSubclassOf<UGameplayEffect> DamageEffect, float Impact)
{
	AActor* OtherActor = OutHit.GetActor();
	UPrimitiveComponent* OtherComp = OutHit.GetComponent();
	if (OtherComp->IsSimulatingPhysics())
	{
		OtherComp->AddImpulseAtLocation(-OutHit.ImpactNormal * Impact, OutHit.Location);
	}
	if (IDamageReceiverInterface* DamageReceiver = Cast<IDamageReceiverInterface>(OtherActor))
	{
		DamageReceiver->ReceiveDamage(OutHit.Location, -OutHit.ImpactNormal * Impact);
	}
	APawn* Instigator = Cast<APawn>(GetOwner());
	UAbilitySystemComponent* SourceASComponent = Instigator->FindComponentByClass<UAbilitySystemComponent>();
	UAbilitySystemComponent* TargetASComponent = OtherActor->FindComponentByClass<UAbilitySystemComponent>();
	if (SourceASComponent && TargetASComponent)
	{
		FGameplayEffectContextHandle EffectContext = SourceASComponent->MakeEffectContext();
		EffectContext.AddSourceObject(Instigator);
		FGameplayEffectSpecHandle SpecHandle = SourceASComponent->MakeOutgoingSpec(DamageEffect, 1, EffectContext);
		SourceASComponent->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data.Get(), TargetASComponent);
	}
}

int32 UTP_WeaponComponent::FindWeaponData(ULevelUpGameplayAbility* Ability, FWeaponData& Data) const
{
	for (int32 i = 0; i < WeaponSlotsData.Num(); ++i)
	{
		if (WeaponSlotsData[i].Ability.Get() == Ability->GetClass())
		{
			Data = WeaponSlotsData[i];
			return i;
		}
	}
	return -1;
}

void UTP_WeaponComponent::LoadWeaponData()
{
	FString OwnerName = IsValid(GetOwner()) ? GetOwner()->GetActorNameOrLabel() : TEXT("NONE");
	FString SkeletalMeshName = IsValid(GetSkinnedAsset()) ? GetSkinnedAsset()->GetName() : TEXT("NONE");
	checkf(WeaponDataTable, TEXT("[Owner:%s][Skel:%s] WeaponDataTable wasn't setup"), *OwnerName, *SkeletalMeshName);
	static const FString ContextString(TEXT("Weapon Data"));
	for (const FName& Slot : WeaponSlots)
	{
		if (FWeaponData* Data = WeaponDataTable->FindRow<FWeaponData>(Slot, ContextString, true))
		{
			WeaponSlotsData.Add(*Data);
		}
	}
	if (WeaponSlotsData.Num() == 0)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find weapon slots data"));
	}
	// Load all required assets 
	for (FWeaponData& Data : WeaponSlotsData)
	{
		Data.ProjectileClass.LoadSynchronous();
		Data.RayTraceFx.LoadSynchronous();
		Data.Ability.LoadSynchronous();
		Data.DamageEffect.LoadSynchronous();
	}
}

void UTP_WeaponComponent::Client_DestroyFakeProjectile_Implementation(int64 ProjectilePtr)
{
	ALevelUpProjectile* Projectile = reinterpret_cast<ALevelUpProjectile*>(ProjectilePtr);
	if (IsValid(Projectile))
	{
		Projectile->Destroy();
	}
}

void UTP_WeaponComponent::Server_ThrowProjectile_Implementation(const FClientProjectileData& ProjectileData)
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
	FPredictProjectilePathParams Params(ProjectileData.Data.Radius, ProjectileData.LaunchRay.Origin, ProjectileData.LaunchRay.Dir * ProjectileData.Data.LaunchVelocity, LifeTime);
	Params.bTraceWithCollision = true;
	Params.bTraceComplex = false;
	Params.ActorsToIgnore = {GetOwner()};
	Params.TraceChannel = ECollisionChannel::ECC_Visibility;
	FPredictProjectilePathResult PredictResult;
	bool bHit = UGameplayStatics::PredictProjectilePath(this, Params, PredictResult);
	int32 PathDataNum = PredictResult.PathData.Num();
	FPredictProjectilePathPointData LastPathData(ProjectileData.LaunchRay.Origin, ProjectileData.LaunchRay.Dir + ProjectileData.Data.LaunchVelocity, 0.0f);
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
																					ProjectileData.LaunchRay.Origin, 
																					ProjectileData.LaunchRay.Dir.Rotation(), 
																					SpawnParameters);
		Projectile->SimulatePath(ProjectileData.TimeStamp);
		Client_DestroyFakeProjectile(ProjectileData.ProjectilePtr);
	}
}