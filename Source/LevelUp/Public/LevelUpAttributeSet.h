// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "AttributeSetMacro.h"

#include "LevelUpAttributeSet.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnHealthChanged);

UCLASS()
class LEVELUP_API ULevelUpAttributeSet : public UAttributeSet
{
	GENERATED_BODY()

public:
	ULevelUpAttributeSet();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;
	void PreAttributeChange(const FGameplayAttribute& Attribute, float& NewValue) override;
	void PostAttributeChange(const FGameplayAttribute& Attribute, float OldValue, float NewValue) override;

public:
	FOnHealthChanged OnHealthChanged;
	FOnHealthChanged OnDeath;

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MaxHealth)
	FGameplayAttributeData MaxHealth;
	LEVELUP_ATTRIBUTE_ACCESSORS(ULevelUpAttributeSet, MaxHealth);
	UFUNCTION()
	virtual void OnRep_MaxHealth(const FGameplayAttributeData& OldMaxHealth);

	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_Health)
	FGameplayAttributeData Health;
	LEVELUP_ATTRIBUTE_ACCESSORS(ULevelUpAttributeSet, Health);
	UFUNCTION()
	virtual void OnRep_Health(const FGameplayAttributeData& OldHealth);
	
	UPROPERTY(BlueprintReadOnly, Category = "Attributes", ReplicatedUsing = OnRep_MovingSpeed)
	FGameplayAttributeData MovingSpeed;
	LEVELUP_ATTRIBUTE_ACCESSORS(ULevelUpAttributeSet, MovingSpeed);
	UFUNCTION()
	virtual void OnRep_MovingSpeed(const FGameplayAttributeData& OldMovingSpeed);
};