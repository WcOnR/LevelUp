// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AbilitySystemInterface.h"

#include "BreakableActor.generated.h"

class UAbilitySystemComponent;
class ULevelUpAttributeSet;
class UGameplayEffect;

UCLASS()
class LEVELUP_API ABreakableActor : public AActor, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABreakableActor();

	void GetLifetimeReplicatedProps(TArray<class FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	void BeginPlay() override;
	void InitializeAttributes();
	UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void UpdateHealthMaterial();

	UFUNCTION()
	void OnHealthChanged();

	UFUNCTION()
	void OnDeath();

	UFUNCTION()
	void OnRep_HealthPercentage();

protected:
	UPROPERTY(VisibleDefaultsOnly, Category = Mesh)
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleDefaultsOnly, Category = Component)
	UAbilitySystemComponent* AbilitySystemComponent;

	UPROPERTY(VisibleDefaultsOnly, Category = AttributeSet)
	ULevelUpAttributeSet* LevelUpAttributeSet;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Effects, meta = (AllowPrivateAccess = "true"))
	TArray<TSubclassOf<UGameplayEffect>> DefaultAttributeEffects;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Material, meta = (AllowPrivateAccess = "true"))
	UMaterialInterface* HealthMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicHealthMaterial;

	UPROPERTY(ReplicatedUsing = OnRep_HealthPercentage)
	float HealthPercentage;
};
