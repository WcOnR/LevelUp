#include "ToolKit/SColorThemePicker.h"

#include "Widgets/Colors/SColorWheel.h"
#include "Widgets/Colors/SColorSpectrum.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Colors/SSimpleGradient.h"
#include "Widgets/Input/SSlider.h"


/** The max time allowed for updating before we shut off auto-updating */
const double SColorThemePicker::MAX_ALLOWED_UPDATE_TIME = 0.1;

#define LOCTEXT_NAMESPACE "ColorThemePicker"

void SColorThemePicker::Construct(const FArguments& InArgs)
{
	TargetColorAttribute = InArgs._TargetColorAttribute;
	CurrentColorHSV = TargetColorAttribute.Get();
	CurrentColorHSV.A = 1.0f;
	OnColorCommitted = InArgs._OnColorCommitted;

	bIsInteractive = false;
	bPerfIsTooSlowToUpdate = false;

	// The height of the gradient bars beneath the sliders
	const FSlateFontInfo SmallLayoutFont = FAppStyle::Get().GetFontStyle("ColorPicker.Font");

	// clang-format off
	this->ChildSlot
	[
		SNew(SVerticalBox)
			
		+ SVerticalBox::Slot()
		.HAlign(HAlign_Left)
		.AutoHeight()
		.Padding(20.0f, 8.0f, 20.0f, 0.0f)
		[
			SNew(SHorizontalBox)

			+ SHorizontalBox::Slot()
			.HAlign(HAlign_Left)
			.AutoWidth()
			[
				SNew(SBox)
				.HAlign(HAlign_Left)
				.VAlign(VAlign_Fill)
				.WidthOverride(125.0f)
				.HeightOverride(125.0f)
				.MinDesiredWidth(125.0f)
				.MaxDesiredWidth(125.0f)
				[
					SNew(SColorWheel)
						.SelectedColor(this, &SColorThemePicker::GetCurrentColor)
						.OnValueChanged(this, &SColorThemePicker::HandleColorSpectrumValueChanged)
						.OnMouseCaptureBegin(this, &SColorThemePicker::HandleInteractiveChangeBegin)
						.OnMouseCaptureEnd(this, &SColorThemePicker::HandleInteractiveChangeEnd)
				]
			]

			+ SHorizontalBox::Slot()
			.AutoWidth()
			[
				// value slider
				SNew(SOverlay)
				+ SOverlay::Slot()
				.Padding(FMargin(4.0f, 0.0f))
				[
					SNew(SSimpleGradient)
						.EndColor(FLinearColor(0.0f, 0.0f, 0.0f, 1.0f).HSVToLinearRGB())
						.StartColor(FLinearColor(0.0f, 0.0f, 1.0f, 1.0f).HSVToLinearRGB())
						.Orientation(Orient_Horizontal)
				]

				+ SOverlay::Slot()
				[
					SNew(SSlider)
						.IndentHandle(false)
						.Orientation(Orient_Vertical)
						.SliderBarColor(FLinearColor::Transparent)
						.Style(&FAppStyle::Get().GetWidgetStyle<FSliderStyle>("ColorPicker.Slider"))
						.Value(CurrentColorHSV.B)
						.OnValueChanged(const_cast<SColorThemePicker*>(this), &SColorThemePicker::HandleColorSpinBoxValueChanged)
				]
			]
		]
				
		+ SVerticalBox::Slot()
		.Padding(20.0f, 8.0f, 0.0f, 0.0f)
		.HAlign(HAlign_Left)
		.AutoHeight()
		[
			SNew(STextBlock)
				.Text(LOCTEXT("NewColorLabel", "Color"))
		]
				
		+ SVerticalBox::Slot()
		.Padding(20.0f, 0.0f, 0.0f, 0.0f)
		.HAlign(HAlign_Left)
		.AutoHeight()
		[
			SNew(SBox)
			.HeightOverride(25.0f)
			.WidthOverride(125.0f)
			[
				// new color
				SNew(SColorBlock) 
					.ColorIsHSV(true) 
					.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
					.ToolTipText(LOCTEXT("NewColorToolTip", "New color without alpha"))
					.Color(this, &SColorThemePicker::GetCurrentColor)
					.UseSRGB(false)
					.Cursor(EMouseCursor::Default)
			]
		]
	];
	// clang-format on
}

void SColorThemePicker::HandleColorSpectrumValueChanged(FLinearColor NewValue)
{
	CurrentColorHSV = NewValue;

	ApplyNewTargetColor();
}

void SColorThemePicker::HandleColorSpinBoxValueChanged(float NewValue)
{
	int32 ComponentIndex = 2;
	if (FMath::IsNearlyEqual(NewValue, CurrentColorHSV.Component(ComponentIndex), KINDA_SMALL_NUMBER))
	{
		return;
	}

	CurrentColorHSV.Component(ComponentIndex) = NewValue;
	ApplyNewTargetColor();
}

void SColorThemePicker::HandleInteractiveChangeBegin()
{
	bIsInteractive = true;
}

void SColorThemePicker::HandleInteractiveChangeEnd()
{
	HandleInteractiveChangeEnd(0.0f);
}

void SColorThemePicker::HandleInteractiveChangeEnd(float NewValue)
{
	bIsInteractive = false;

	UpdateColorPickMouseUp();
}

bool SColorThemePicker::ApplyNewTargetColor()
{
	bool bUpdated = false;

	if (!bPerfIsTooSlowToUpdate)
	{
		double StartUpdateTime = FPlatformTime::Seconds();
		UpdateColorPickMouseUp();
		double EndUpdateTime = FPlatformTime::Seconds();

		if (EndUpdateTime - StartUpdateTime > MAX_ALLOWED_UPDATE_TIME)
		{
			bPerfIsTooSlowToUpdate = true;
		}

		bUpdated = true;
	}

	return bUpdated;
}

void SColorThemePicker::UpdateColorPick()
{
	bPerfIsTooSlowToUpdate = false;
	FLinearColor OutColor = CurrentColorHSV;
	OnColorCommitted.ExecuteIfBound(OutColor);

	// This callback is only necessary for wx backwards compatibility
	FCoreDelegates::ColorPickerChanged.Broadcast();
}

void SColorThemePicker::UpdateColorPickMouseUp()
{
	UpdateColorPick();
}

#undef LOCTEXT_NAMESPACE
