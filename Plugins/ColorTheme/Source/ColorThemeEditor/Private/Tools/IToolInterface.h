// Copyright Epic Games, Inc. All Rights Reserved.
#pragma once

class IToolInterface
{
public:
	virtual void OnStartupModule() = 0;
	virtual void OnShutdownModule() = 0;
	virtual ~IToolInterface(){}
};
