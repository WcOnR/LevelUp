// Fill out your copyright notice in the Description page of Project Settings.


#include "LocationGATargetActor.h"
#include "Abilities/GameplayAbility.h"

void ALocationGATargetActor::StartTargeting(UGameplayAbility* Ability)
{
	Super::StartTargeting(Ability);
}

void ALocationGATargetActor::ConfirmTargetingAndContinue()
{
	if (IsConfirmTargetingAllowed())
	{
		FGameplayAbilityTargetDataHandle Handle;
		FGameplayAbilityTargetData_TransformInfo* ReturnData = new FGameplayAbilityTargetData_TransformInfo();
		ReturnData->SourceLocation = StartLocation;
		Handle.Add(ReturnData);
		TargetDataReadyDelegate.Broadcast(Handle);
	}
}

bool FGameplayAbilityTargetData_TransformInfo::NetSerialize(FArchive& Ar, UPackageMap* Map, bool& bOutSuccess)
{
	SourceLocation.NetSerialize(Ar, Map, bOutSuccess);

	bOutSuccess = true;
	return true;
}
