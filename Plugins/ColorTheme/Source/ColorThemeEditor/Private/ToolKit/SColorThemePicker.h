#pragma once

#include "CoreMinimal.h"
#include "Misc/Attribute.h"
#include "Input/Reply.h"
#include "Layout/Visibility.h"
#include "Widgets/DeclarativeSyntaxSupport.h"
#include "Widgets/SWidget.h"
#include "Widgets/SCompoundWidget.h"
#include "Widgets/SWindow.h"
#include "Framework/SlateDelegates.h"


class SColorThemePicker : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(SColorThemePicker)
		: _TargetColorAttribute(FLinearColor(ForceInit))
		, _OnColorCommitted()
	{
	}

	/** The color that is being targeted as a TAttribute */
	SLATE_ATTRIBUTE(FLinearColor, TargetColorAttribute)

	/** The event called when the color is committed */
	SLATE_EVENT(FOnLinearColorValueChanged, OnColorCommitted)

	/** If true, this color picker will have non-standard creation behavior */
	SLATE_ARGUMENT(bool, OverrideColorPickerCreation)

	SLATE_END_ARGS()

public:
	void Construct(const FArguments& InArgs);

protected:
	FLinearColor GetCurrentColor() const { return CurrentColorHSV; }

	// Callback for value changes in the color spectrum picker.
	void HandleColorSpectrumValueChanged(FLinearColor NewValue);

	void HandleColorSpinBoxValueChanged(float NewValue);

	// Callback for when interactive user input begins.
	void HandleInteractiveChangeBegin();

	// Callback for when interactive user input ends.
	void HandleInteractiveChangeEnd();

	void HandleInteractiveChangeEnd(float NewValue);

	bool ApplyNewTargetColor();
	
	void UpdateColorPick();
	void UpdateColorPickMouseUp();

private:
	/** The color that is being targeted as a TAttribute */
	TAttribute<FLinearColor> TargetColorAttribute;

	/** The current color being picked in HSV */
	FLinearColor CurrentColorHSV;

	/** The max time allowed for updating before we shut off auto-updating */
	static const double MAX_ALLOWED_UPDATE_TIME;

	/** If true, then the performance is too bad to have auto-updating */
	bool bPerfIsTooSlowToUpdate;

	/** Tracks whether the user is moving a value spin box, the color wheel and the dropper */
	bool bIsInteractive;

	/** Invoked when a new value is selected on the color wheel */
	FOnLinearColorValueChanged OnColorCommitted;
};