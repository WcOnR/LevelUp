// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/SGraphPinValueByProperty.h"

void SGraphPinValueByProperty::Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj, const UObject* InObject)
{
	UpdateTargetObject(InObject);
	SGraphPinNameList::Construct(SGraphPinNameList::FArguments(), InGraphPinObj, NameList);
}

void SGraphPinValueByProperty::UpdateTargetObject(const UObject* InObject)
{
	if (CachedObject == InObject)
	{
		return;
	}

	CachedObject = InObject;
	RefreshNameList();
}

void SGraphPinValueByProperty::RefreshNameList()
{
	NameList.Empty();
	if (IsValid(CachedObject))
	{
		UClass* ObjectClass = Cast<UClass>(CachedObject);
		for (TFieldIterator<FProperty> PropertyIterator(ObjectClass); PropertyIterator; ++PropertyIterator)
		{
			FProperty* Property = *PropertyIterator;
			TSharedPtr<FName> PropertyNameItem = MakeShareable(new FName(Property->GetName()));
			NameList.Add(PropertyNameItem);
		}
	}
}
