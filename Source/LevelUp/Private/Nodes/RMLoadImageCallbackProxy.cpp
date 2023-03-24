// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nodes/RMLoadImageCallbackProxy.h"

#include UE_INLINE_GENERATED_CPP_BY_NAME(RMLoadImageCallbackProxy)

//////////////////////////////////////////////////////////////////////////
// URMLoadImageCallbackProxy

URMLoadImageCallbackProxy::URMLoadImageCallbackProxy(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

URMLoadImageCallbackProxy* URMLoadImageCallbackProxy::CreateProxyObjectForRMLoadImage(int32 CharacterIndex)
{
	URMLoadImageCallbackProxy* Proxy = NewObject<URMLoadImageCallbackProxy>();
	Proxy->SetFlags(RF_StrongRefOnFrame);
	Proxy->RMLoadImage(CharacterIndex);
	return Proxy;
}

void URMLoadImageCallbackProxy::RMLoadImage(int32 CharacterIndex)
{
	if (CharacterIndex > 0)
	{
		OnSucceed.Broadcast(NAME_None);
	}
	else
	{
		OnFailed.Broadcast(NAME_None);
	}
}