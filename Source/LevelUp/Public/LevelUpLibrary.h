// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "Engine/LatentActionManager.h"
#include "LatentActions.h"
#include "InputCoreTypes.h"
#include "LevelUpLibrary.generated.h"

class APlayerController;

class FWaitGameplayInputAction : public FPendingLatentAction
{
public:
	FKey Key;
	APlayerController* Controller;
	FName ExecutionFunction;
	int32 OutputLink;
	FWeakObjectPtr CallbackTarget;

	FWaitGameplayInputAction(FKey TargetKey, APlayerController* PlayerController, const FLatentActionInfo& LatentInfo);

	virtual void UpdateOperation(FLatentResponse& Response) override;
};

namespace LevelUp
{
	template <typename ObjectType>
	static ObjectType* FindOrLoadObject(const FString& ObjectPath)
	{
		ObjectType* Object = FindObject<ObjectType>(nullptr, *ObjectPath);
		if (!Object)
		{
			Object = LoadObject<ObjectType>(nullptr, *ObjectPath);
		}

		return Object;
	}
}


UCLASS()
class LEVELUP_API ULevelUpLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Perform a latent action after the Key was pressed.
	 * Calling again while it is waiting or from server side will be ignored.
	 *
	 * @param WorldContext	World context.
	 * @param Key	 		Key to wait.
	 * @param LatentInfo 	The latent action.
	 */
	UFUNCTION(BlueprintCallable, Category="Utilities|FlowControl", meta=(Latent, WorldContext="WorldContextObject", LatentInfo="LatentInfo", Keywords="input"))
	static void WaitGameplayInput(const UObject* WorldContextObject, FKey Key, struct FLatentActionInfo LatentInfo);

	UFUNCTION(BlueprintCallable, CustomThunk, meta = (CustomStructureParam = "OutValue", BlueprintInternalUseOnly = "true"))
	static bool GetValueByPropertyName(UObject* Target, FName PropertyName, int32& OutValue);

private:
	DECLARE_FUNCTION(execGetValueByPropertyName);
};