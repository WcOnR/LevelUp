// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

class FValueByPropertyPinFactory;

class FLevelUpModule : public FDefaultGameModuleImpl
{
public:
	void StartupModule() override;
	void ShutdownModule() override;

protected:
	TSharedPtr<class FValueByPropertyPinFactory> ValueByPropertyPinFactory;
};
