// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "InputActionValue.h"
#include "AbilitySystemInterface.h"

#include "LevelUpCharacter.generated.h"

class UInputComponent;
class UEnhancedInputComponent;
class USkeletalMeshComponent;
class UAbilityInputMapDataAsset;
class UAbilitySystemComponent;
class ULevelUpAttributeSet;
class UWeaponAttributeSet;
class ULevelUpGameplayAbility;
class UGameplayEffect;
class UTP_WeaponComponent;
class USceneComponent;
class UCameraComponent;
class UGrenadeLauncherComponent;
class UPathPredictComponent;

UCLASS(config = Game)
class ALevelUpCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	ALevelUpCharacter();

	/** Returns Mesh1P subobject **/
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UWeaponAttributeSet* GetWeaponAttributeSet() const { return WeaponAttributeSet; }

	void AddWeaponAbilities(const TArray<TSubclassOf<ULevelUpGameplayAbility>>& WeaponAbilities);

protected:
	void BeginPlay() override;
	void PossessedBy(AController* NewController) override;
	void OnRep_PlayerState() override;

	/** Called for movement input */
	void Move(const FInputActionValue& Value);

	/** Called for looking input */
	void Look(const FInputActionValue& Value);

	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void InitializeAbilities();

	void BindAbilityAction(UEnhancedInputComponent* EnhancedInputComponent, TSubclassOf<ULevelUpGameplayAbility> Ability, int32 Id);

private:
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UTP_WeaponComponent* Weapon;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UGrenadeLauncherComponent* GrenadeLauncher;

	UPROPERTY(BlueprintReadOnly, VisibleDefaultsOnly, Category = Component, meta = (AllowPrivateAccess = "true"))
	UPathPredictComponent* PathPredict;

	UPROPERTY(VisibleDefaultsOnly, Category = AttributeSet)
	ULevelUpAttributeSet* LevelUpAttributeSet;

	UPROPERTY(VisibleDefaultsOnly, Category = AttributeSet)
	UWeaponAttributeSet* WeaponAttributeSet;

	/** MappingContext */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputMappingContext* DefaultMappingContext;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* MoveAction;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ULevelUpGameplayAbility>> DefaultAbilities;
};
