// Copyright Epic Games, Inc. All Rights Reserved.

#include "Nodes/K2Node_RMLoadImage.h"

#include "Containers/UnrealString.h"
#include "EdGraph/EdGraphPin.h"
#include "HAL/Platform.h"
#include "Internationalization/Internationalization.h"
#include "Misc/AssertionMacros.h"
#include "Nodes/RMLoadImageCallbackProxy.h"
#include "UObject/NameTypes.h"
#include "UObject/ObjectPtr.h"

#define LOCTEXT_NAMESPACE "K2Node"

UK2Node_RMLoadImage::UK2Node_RMLoadImage(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	ProxyFactoryFunctionName = GET_FUNCTION_NAME_CHECKED(URMLoadImageCallbackProxy, CreateProxyObjectForRMLoadImage);
	ProxyFactoryClass = URMLoadImageCallbackProxy::StaticClass();
	ProxyClass = URMLoadImageCallbackProxy::StaticClass();
}

FText UK2Node_RMLoadImage::GetTooltipText() const
{
	return LOCTEXT("K2Node_RMLoadImage_Tooltip", "Load Rick and Morty Image");
}

FText UK2Node_RMLoadImage::GetNodeTitle(ENodeTitleType::Type TitleType) const
{
	return LOCTEXT("RMLoadImage", "Load Image");
}

FText UK2Node_RMLoadImage::GetMenuCategory() const
{
	return LOCTEXT("RMLoadImageCategory", "Animation|Montage");
}

void UK2Node_RMLoadImage::GetPinHoverText(const UEdGraphPin& Pin, FString& HoverTextOut) const
{
	Super::GetPinHoverText(Pin, HoverTextOut);

	static const FName NAME_OnSucceed = FName(TEXT("OnSucceed"));
	static const FName NAME_OnFailed = FName(TEXT("OnFailed"));

	if (Pin.PinName == NAME_OnSucceed)
	{
		FText ToolTipText = LOCTEXT("K2Node_RMLoadImage_OnSucceed_Tooltip", "Event called when loading succeed.");
		HoverTextOut = FString::Printf(TEXT("%s\n%s"), *ToolTipText.ToString(), *HoverTextOut);
	}
	else if (Pin.PinName == NAME_OnFailed)
	{
		FText ToolTipText = LOCTEXT("K2Node_RMLoadImage_OnFailed_Tooltip", "Event called when loading failed.");
		HoverTextOut = FString::Printf(TEXT("%s\n%s"), *ToolTipText.ToString(), *HoverTextOut);
	}
}

#undef LOCTEXT_NAMESPACE