// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "SGraphPinNameList.h"


class LEVELUP_API SGraphPinValueByProperty : public SGraphPinNameList
{
public:
	SLATE_BEGIN_ARGS(SGraphPinValueByProperty) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs, UEdGraphPin* InGraphPinObj, const UObject* InObject);

	void UpdateTargetObject(const UObject* InObject);

protected:
	void RefreshNameList();

	TObjectPtr<const UObject> CachedObject;
};