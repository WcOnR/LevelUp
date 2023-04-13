// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbilityTargetActor.h"
#include "LocationGATargetActor.generated.h"


/** Target data with just a source and target location in space */
USTRUCT(BlueprintType)
struct LEVELUP_API FGameplayAbilityTargetData_TransformInfo : public FGameplayAbilityTargetData
{
	GENERATED_USTRUCT_BODY()

	/** Generic location data for source */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = Targeting)
	FGameplayAbilityTargetingLocationInfo SourceLocation;

	// -------------------------------------
	bool HasOrigin() const override { return true; }

	FTransform GetOrigin() const override { return SourceLocation.GetTargetingTransform(); }

	// -------------------------------------

	bool HasEndPoint() const override { return true; }

	FVector GetEndPoint() const override { return SourceLocation.GetTargetingTransform().GetLocation(); }

	FTransform GetEndPointTransform() const override { return SourceLocation.GetTargetingTransform(); }

	// -------------------------------------

	UScriptStruct* GetScriptStruct() const override { return FGameplayAbilityTargetData_TransformInfo::StaticStruct(); }

	FString ToString() const override { return TEXT("FGameplayAbilityTargetData_TransformInfo"); }

	bool NetSerialize(FArchive& Ar, class UPackageMap* Map, bool& bOutSuccess);
};

template<>
struct TStructOpsTypeTraits<FGameplayAbilityTargetData_TransformInfo> : public TStructOpsTypeTraitsBase2<FGameplayAbilityTargetData_TransformInfo>
{
	enum
	{
		WithNetSerializer = true	// For now this is REQUIRED for FGameplayAbilityTargetDataHandle net serialization to work
	};
};

UCLASS()
class LEVELUP_API ALocationGATargetActor : public AGameplayAbilityTargetActor
{
	GENERATED_BODY()
	
public:
	void StartTargeting(UGameplayAbility* Ability) override;

	void ConfirmTargetingAndContinue() override;
};
