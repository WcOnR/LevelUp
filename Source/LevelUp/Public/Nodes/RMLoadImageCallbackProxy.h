// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

#include "RMLoadImageCallbackProxy.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoadImageDelegate, FName, NotifyName);

UCLASS(MinimalAPI)
class URMLoadImageCallbackProxy : public UObject
{
	GENERATED_UCLASS_BODY()

	UPROPERTY(BlueprintAssignable)
	FOnLoadImageDelegate OnSucceed;

	UPROPERTY(BlueprintAssignable)
	FOnLoadImageDelegate OnFailed;

	// Called to perform the query internally
	UFUNCTION(BlueprintCallable, meta = (BlueprintInternalUseOnly = "true"))
	static URMLoadImageCallbackProxy* CreateProxyObjectForRMLoadImage(int32 CharacterIndex);

	void RMLoadImage(int32 CharacterIndex);
};
