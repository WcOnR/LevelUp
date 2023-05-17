// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpLibrary.h"

#include "GameFramework/PlayerController.h"


FWaitGameplayInputAction::FWaitGameplayInputAction(FKey TargetKey, APlayerController* PlayerController, const FLatentActionInfo& LatentInfo)
	: Key(TargetKey)
	, Controller(PlayerController)
	, ExecutionFunction(LatentInfo.ExecutionFunction)
	, OutputLink(LatentInfo.Linkage)
	, CallbackTarget(LatentInfo.CallbackTarget)
{
}

inline void FWaitGameplayInputAction::UpdateOperation(FLatentResponse& Response)
{
	if (!IsValid(Controller))
	{
		Response.DoneIf(true);
		return;
	}
	bool InputPressed = Controller->WasInputKeyJustPressed(Key);
	Response.FinishAndTriggerIf(InputPressed, ExecutionFunction, OutputLink, CallbackTarget);
}

void ULevelUpLibrary::WaitGameplayInput(const UObject* WorldContextObject, FKey Key, FLatentActionInfo LatentInfo)
{
	if (UWorld* World = GEngine->GetWorldFromContextObject(WorldContextObject, EGetWorldErrorMode::LogAndReturnNull))
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (IsValid(PlayerController) && !PlayerController->IsLocalController()) // Drop execution on server
		{
			PlayerController = nullptr;
		}
		FLatentActionManager& LatentActionManager = World->GetLatentActionManager();
		if (LatentActionManager.FindExistingAction<FWaitGameplayInputAction>(LatentInfo.CallbackTarget, LatentInfo.UUID) == NULL)
		{
			LatentActionManager.AddNewAction(LatentInfo.CallbackTarget, LatentInfo.UUID, new FWaitGameplayInputAction(Key, PlayerController, LatentInfo));
		}
	}
}