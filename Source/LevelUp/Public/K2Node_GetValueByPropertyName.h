// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#include "UObject/ObjectMacros.h"

#include "K2Node.h"
#include "K2Node_GetValueByPropertyName.generated.h"

class FBlueprintActionDatabaseRegistrar;
class UBlueprint;
class UEdGraph;
class UEdGraphPin;
class UProperty;
class SGraphPinValueByProperty;
class FValueByPropertyPinFactory;

/**
 *
*/
UCLASS(BlueprintType, Blueprintable)
class LEVELUP_API UK2Node_GetValueByPropertyName : public UK2Node
{
	GENERATED_BODY()

	friend FValueByPropertyPinFactory;

public:
	//~ Begin UEdGraphNode Interface.
	void AllocateDefaultPins() override;
	void PostReconstructNode() override;
	void PinDefaultValueChanged(UEdGraphPin* Pin) override;
	FText GetNodeTitle(ENodeTitleType::Type TitleType) const override;
	FText GetTooltipText() const override;
	//~ End UEdGraphNode Interface.

	//~ Begin UK2Node Interface
	bool NodeCausesStructuralBlueprintChange() const override { return true; }
	void NotifyPinConnectionListChanged(UEdGraphPin* Pin) override;
	bool IsConnectionDisallowed(const UEdGraphPin* MyPin, const UEdGraphPin* OtherPin, FString& OutReason) const override;
	FText GetMenuCategory() const override;
	void GetMenuActions(FBlueprintActionDatabaseRegistrar& ActionRegistrar) const override;
	void ExpandNode(class FKismetCompilerContext& CompilerContext, UEdGraph* SourceGraph) override;
	FSlateIcon GetIconAndTint(FLinearColor& OutColor) const override;
	//~ End UK2Node Interface

protected:
	void SetPropertyNameGraphPin(TSharedPtr<SGraphPinValueByProperty> GraphPin);
	void SetPinToolTip(UEdGraphPin& MutatablePin, const FText& PinDescription) const;
	void RefreshOutValuePinType(bool bRefreshGraphPinSlate);

	void OnTargetOrNamePinChanged(UEdGraphPin* Pin);

	UObject* GetTarget() const;
	FName GetPropertyName() const;
	FEdGraphPinType GetObjectPropertyPinType(const UObject* TargetObject) const;

	UEdGraphPin* GetThenPin() const;
	UEdGraphPin* GetTargetPin() const;
	UEdGraphPin* GetPropertyNamePin() const;
	UEdGraphPin* GetSuccessPin() const;
	UEdGraphPin* GetReturnValuePin() const;

protected:
	TSharedPtr<SGraphPinValueByProperty> PropertyNameGraphPin;
};