#include "ToolKit/SColorThemeApplyer.h"

#include "Settings/EditorStyleSettings.h"
#include "Styling/StyleColors.h"
#include "Widgets/Colors/SColorBlock.h"
#include "Widgets/Input/SSlider.h"

#define LOCTEXT_NAMESPACE "ColorThemePicker"

void SColorThemeApplyer::Construct(const FArguments& InArgs)
{
	ColorHSV = InArgs._Color.Get();
	const FSlateFontInfo SmallLayoutFont = FAppStyle::Get().GetFontStyle("ColorPicker.Font");
	FText ToolTipText = LOCTEXT("SelectedColorToolTip", "Target theme color");

	// clang-format off
	this->ChildSlot
	[
		SNew(SBox)
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.WidthOverride(100.0f)
		.HeightOverride(100.0f)
		[
			SNew(SVerticalBox)
			+SVerticalBox::Slot()
			.Padding(10.0f, 10.0f, 10.0f, 0.0f)
			.VAlign( VAlign_Top )
			.HAlign( HAlign_Fill )
			.AutoHeight()
			[
				SNew(SBox)
				.WidthOverride(80.0f)
				.HeightOverride(80.0f)
				[
					SNew(SColorBlock) 
					.ColorIsHSV(true) 
					.AlphaDisplayMode(EColorBlockAlphaDisplayMode::Ignore)
					.ToolTipText(ToolTipText)
					.Color(ColorHSV)
					.UseSRGB(false)
					.Cursor(EMouseCursor::Default)
				]
			]
			+SVerticalBox::Slot()
			.Padding(10.0f, 20.0f, 10.0f, 10.0f)
			.VAlign( VAlign_Bottom )
			.HAlign( HAlign_Right )
			.AutoHeight()
			[
				SNew(SButton)
				.Text(LOCTEXT("Apply", "Apply theme"))
				.OnClicked(this, &SColorThemeApplyer::OnApply)
			]
		]
	];
	// clang-format on
}

FReply SColorThemeApplyer::OnApply() const
{
	UEditorStyleSettings* StyleSettings = GetMutableDefault<UEditorStyleSettings>();
	StyleSettings->SelectionColor = ColorHSV.HSVToLinearRGB();
	StyleSettings->SaveConfig();
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
