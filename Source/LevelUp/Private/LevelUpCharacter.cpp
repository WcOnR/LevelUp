// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpCharacter.h"

#include <GameplayEffectTypes.h>
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "TP_WeaponComponent.h"
#include "AbilitySystemComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "LevelUpProjectile.h"
#include "LevelUpAttributeSet.h"
#include "WeaponAttributeSet.h"
#include "LevelUpGameplayAbility.h"

//////////////////////////////////////////////////////////////////////////
// ALevelUpCharacter

ALevelUpCharacter::ALevelUpCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f));	   // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	AbilitySystemComponent = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	LevelUpAttributeSet = CreateDefaultSubobject<ULevelUpAttributeSet>(TEXT("CommonAttributeSet"));
	WeaponAttributeSet = CreateDefaultSubobject<UWeaponAttributeSet>(TEXT("WeaponAttributeSet"));
	
	Weapon = CreateDefaultSubobject<UTP_WeaponComponent>(TEXT("TP_WeaponComponent"));
	Weapon->SetupAttachment(Mesh1P, FName(TEXT("GripPoint")));
}

void ALevelUpCharacter::BeginPlay()
{
	Super::BeginPlay();
	// Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}
	Weapon->AttachWeapon(this);
}

void ALevelUpCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	if (AbilitySystemComponent)
	{
		AbilitySystemComponent->InitAbilityActorInfo(this, this);
		InitializeAttributes();
		InitializeAbilities();
	}

	SetOwner(NewController);
}

void ALevelUpCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();
	InitializeAttributes();
}

//////////////////////////////////////////////////////////////////////////// Input

void ALevelUpCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add movement
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ALevelUpCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ALevelUpCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		for (int32 i = 0; i < DefaultAbilities.Num(); ++i)
		{
			const TSubclassOf<ULevelUpGameplayAbility>& Ability = DefaultAbilities[i];
			UInputAction* Input = Ability.GetDefaultObject()->Input;
			EnhancedInputComponent->BindAction(Input, ETriggerEvent::Triggered, AbilitySystemComponent, &UAbilitySystemComponent::AbilityLocalInputPressed, i);
			EnhancedInputComponent->BindAction(Input, ETriggerEvent::Completed, AbilitySystemComponent, &UAbilitySystemComponent::AbilityLocalInputReleased, i);
		}

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ALevelUpCharacter::Move);

		// Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ALevelUpCharacter::Look);
	}
}

//////////////////////////////////////////////////////////////////////////// Ability System

 UAbilitySystemComponent* ALevelUpCharacter::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void ALevelUpCharacter::InitializeAttributes()
{
	if (AbilitySystemComponent && DefaultAttributeEffect)
	{
		FGameplayEffectContextHandle EffectContext = AbilitySystemComponent->MakeEffectContext();
		EffectContext.AddSourceObject(this);

		FGameplayEffectSpecHandle SpecHandle = AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributeEffect, 1, EffectContext);

		if (SpecHandle.IsValid())
		{
			FActiveGameplayEffectHandle EffectHandle = AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*SpecHandle.Data.Get());
		}
	}
}

void ALevelUpCharacter::InitializeAbilities()
{
	if (HasAuthority() && AbilitySystemComponent)
	{
		for (int32 i = 0; i < DefaultAbilities.Num(); ++i)
		{
			AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(DefaultAbilities[i], 1, i, this));
		}
	}
}
