// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AttributeSet.h"

#define GAMEPLAYATTRIBUTE_VALUE_SETTER_DEF(PropertyName) \
	void Set##PropertyName##(float NewVal);

#define GAMEPLAYATTRIBUTE_VALUE_SETTER_IMPL(ClassName, PropertyName) \
	FORCEINLINE void ClassName##::Set##PropertyName##(float NewVal) \
	{ \
		UAbilitySystemComponent* AbilityComp = GetOwningAbilitySystemComponent(); \
		if (ensure(AbilityComp)) \
		{ \
			AbilityComp->SetNumericAttributeBase(Get##PropertyName##Attribute(), NewVal); \
		}; \
	}

// Uses macros from AttributeSet.h

#define LEVELUP_ATTRIBUTE_ACCESSORS(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_PROPERTY_GETTER(ClassName, PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_GETTER(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER_DEF(PropertyName) \
	GAMEPLAYATTRIBUTE_VALUE_INITTER(PropertyName)

#define GAMEPLAYATTRIBUTE_VALUE_ATTREBUT_DEF_IMPL(ClassName, AttrebutName) \
	GAMEPLAYATTRIBUTE_VALUE_SETTER_IMPL(ClassName, AttrebutName) \
	void ClassName::OnRep_##AttrebutName##(const FGameplayAttributeData& Old##AttrebutName##) \
	{ \
		GAMEPLAYATTRIBUTE_REPNOTIFY(ClassName, AttrebutName, Old##AttrebutName##); \
	}