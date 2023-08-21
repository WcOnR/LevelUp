// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "DamageReceiverInterface.h"
#include "GenericTeamAgentInterface.h"

#include "EnemyCharacter.generated.h"

class UAbilitySystemComponent;
class ULevelUpAttributeSet;
class ULevelUpGameplayAbility;
class UGameplayEffect;

UCLASS(config = Game)
class LEVELUP_API AEnemyCharacter : public ACharacter,
									public IAbilitySystemInterface,
									public IDamageReceiverInterface,
									public IGenericTeamAgentInterface
{
	GENERATED_BODY()

public:
	AEnemyCharacter();

	FGenericTeamId GetGenericTeamId() const override;

protected:
	void BeginPlay() override;

	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	virtual void InitializeAttributes();
	virtual void InitializeAbilities();

	UFUNCTION()
	void OnDeath();

	void ReceiveDamage(const FVector& Location, const FVector& Velocity) override;

private:
	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = AttributeSet)
	ULevelUpAttributeSet* AttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Abilities, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<ULevelUpGameplayAbility>> DefaultAbilities;

	TPair<FVector, FVector> LastReceivedDamage;
};
