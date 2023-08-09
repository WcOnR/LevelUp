// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUp.h"
#include "Modules/ModuleManager.h"

#include "ValueByPropertyPinFactory.h"
#include <Subsystems/EditorActorSubsystem.h>
#include <UnrealEd.h>

IMPLEMENT_PRIMARY_GAME_MODULE(FLevelUpModule, LevelUp, "LevelUp");

void FLevelUpModule::StartupModule()
{
	ValueByPropertyPinFactory = MakeShared<FValueByPropertyPinFactory>();
	FEdGraphUtilities::RegisterVisualPinFactory(ValueByPropertyPinFactory);
}

void FLevelUpModule::ShutdownModule()
{
	if (UObjectInitialized() && !IsEngineExitRequested())
	{
		FEdGraphUtilities::UnregisterVisualPinFactory(ValueByPropertyPinFactory);
	}
}