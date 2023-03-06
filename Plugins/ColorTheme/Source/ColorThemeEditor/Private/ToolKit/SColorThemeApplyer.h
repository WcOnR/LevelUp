#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"


class SColorThemeApplyer : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SColorThemeApplyer) : _Color(FLinearColor::Black)
	{}

	/** The color to display for this color block */
	SLATE_ATTRIBUTE(FLinearColor, Color)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

protected:
	FReply OnApply() const;

private:
	/** The current color being picked in HSV */
	FLinearColor ColorHSV;
};