// Copyright Epic Games, Inc. All Rights Reserved.

#include "ValueByPropertyPinFactory.h"

#include "KismetPins/SGraphPinExec.h"
#include "K2Node_GetValueByPropertyName.h"
#include "UI/SGraphPinValueByProperty.h"

TSharedPtr<class SGraphPin> FValueByPropertyPinFactory::CreatePin(UEdGraphPin* InPin) const
{
	if (InPin->PinType.PinCategory == UEdGraphSchema_K2::PC_Name)
	{
		UObject* Outer = InPin->GetOuter();
		if (Outer->IsA(UK2Node_GetValueByPropertyName::StaticClass()))
		{
			UK2Node_GetValueByPropertyName* ObjectPropertyValueNode = CastChecked<UK2Node_GetValueByPropertyName>(Outer);
			if (ObjectPropertyValueNode && InPin->Direction == EGPD_Input)
			{
				const UEdGraphPin* ObjectPin = ObjectPropertyValueNode->GetTargetPin();
				if (ObjectPin)
				{
					TSharedPtr<SGraphPinValueByProperty> GraphPin = SNew(SGraphPinValueByProperty, InPin, ObjectPropertyValueNode->GetTarget());
					ObjectPropertyValueNode->SetPropertyNameGraphPin(GraphPin);
					return GraphPin;
				}
			}
		}
	}
	return nullptr;
}
