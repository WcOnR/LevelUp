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

bool Generic_GetValueByPropertyName(UObject* Target, FName PropertyName, void* OutValue)
{
	if (!IsValid(Target))
	{
		return false;
	}

	FProperty* ResultProperty = Target->GetClass()->FindPropertyByName(PropertyName);
	if (!ResultProperty)
	{
		return false;
	}

	void* ValueAddress = ResultProperty->ContainerPtrToValuePtr<void>(Target);
	ResultProperty->CopyCompleteValueFromScriptVM(OutValue, ValueAddress);

	return true;
}

DEFINE_FUNCTION(ULevelUpLibrary::execGetValueByPropertyName)
{
    P_GET_OBJECT(UObject, Target);
    P_GET_PROPERTY(FNameProperty, PropertyName);

    Stack.StepCompiledIn<FStructProperty>(NULL);
    void* OutValuePtr = Stack.MostRecentPropertyAddress;

    P_FINISH;
    bool bSuccess = false;

	bool bOutputResolved = false;
    if (!Target)
    {
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            FText::FromString("Failed to resolve the object input. Be sure the Target is valid.")
        );
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
    }
	else if (OutValuePtr)
	{
		TSet<FName> SimpleTypes = {FBoolProperty::StaticClass()->GetFName(),
								   FEnumProperty::StaticClass()->GetFName(),
								   FNameProperty::StaticClass()->GetFName(),
								   FStrProperty::StaticClass()->GetFName(),
								   FTextProperty::StaticClass()->GetFName()};
		const FFieldClass* OutValueClass = Stack.MostRecentProperty->GetClass();
		const FName OutValueClassName = OutValueClass->GetFName();

		if (Stack.MostRecentProperty->IsA<FNumericProperty>() || Stack.MostRecentProperty->IsA<FObjectPropertyBase>() ||
			SimpleTypes.Contains(OutValueClassName))
		{
			bOutputResolved = true;
			P_NATIVE_BEGIN;
			bSuccess = Generic_GetValueByPropertyName(Target, PropertyName, OutValuePtr);
			P_NATIVE_END;
		}
		else if (FStructProperty::StaticClass()->GetFName() == OutValueClassName)
		{
			bOutputResolved = true;
			const FFieldClass* InnerPropClass = FStructProperty::StaticClass();
			const FFieldClass* MostRecentPropClass = OutValueClass;

			const bool bCompatible = (InnerPropClass == MostRecentPropClass) || 
				MostRecentPropClass->IsChildOf(InnerPropClass) || InnerPropClass->IsChildOf(MostRecentPropClass);
			if (bCompatible)
			{
				P_NATIVE_BEGIN;
				bSuccess = Generic_GetValueByPropertyName(Target, PropertyName, OutValuePtr);
				P_NATIVE_END;
			}
			else
			{
				FBlueprintExceptionInfo ExceptionInfo(
					EBlueprintExceptionType::AccessViolation,
					NSLOCTEXT("GetDataTableRow", "IncompatibleProperty", "Incompatible output parameter; the data table's type is not the same as the return type.")
				);
				FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
			}
		}
	}
	if (!bOutputResolved)
    {
		// FDelegateProperty
		// FMulticastDelegateProperty
		// FInterfaceProperty
		// FNetPropertyHook
		// FArrayProperty
		// FMapProperty
		// FSetProperty
		// WARNING! Above list of properties are not supported
        FBlueprintExceptionInfo ExceptionInfo(
            EBlueprintExceptionType::AccessViolation,
            FText::FromString("Failed to resolve the output parameter for GetValueByPropertyName.")
        );
        FBlueprintCoreDelegates::ThrowScriptException(P_THIS, Stack, ExceptionInfo);
    }

    *(bool*)RESULT_PARAM = bSuccess;
}