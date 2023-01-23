// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SPauseMenuWidget.h"

#include "UI/MainGameHUD.h"
#include "Widgets/Layout/SBackgroundBlur.h"

#define LOCTEXT_NAMESPACE "PauseMenu"

void SPauseMenuWidget::Construct(const FArguments& InArgs)
{
	OwnHUD = InArgs._OwnHUD;
	bCanSupportFocus = true;

	const FMargin ContentPadding = FMargin(750.f, 300.f);
	const FMargin ButtonPadding = FMargin(10.f);

	const FText PauseText = LOCTEXT("PauseTitle", "PAUSE");
	const FText ResumeText = LOCTEXT("ResumeGame", "Resume game");
	const FText QuitText = LOCTEXT("Quit", "Quit");

	FSlateFontInfo PauseTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	PauseTextStyle.Size = 60.f;

	FSlateFontInfo ButtonTextStyle = PauseTextStyle;
	ButtonTextStyle.Size = 40.f;


	// clang-format off
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		[
			SNew(SBackgroundBlur)
			.HAlign(HAlign_Fill)
			.VAlign(VAlign_Fill)
			.BlurStrength(5.f)
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Fill)
		.VAlign(VAlign_Fill)
		.Padding(ContentPadding)
		[
			SNew(SVerticalBox)
			// Pause text
			+ SVerticalBox::Slot()
			[
				SNew(STextBlock)
				.Font(PauseTextStyle)
				.Text(PauseText)
				.Justification(ETextJustify::Center)
			]
			// Resume Button
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				.OnClicked(this, &SPauseMenuWidget::OnResume)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(ResumeText)
					.Justification(ETextJustify::Center)
				]
				
			]
			// Quit Button
			+ SVerticalBox::Slot()
			.Padding(ButtonPadding)
			[
				SNew(SButton)
				.OnClicked(this, &SPauseMenuWidget::OnQuit)
				.VAlign(VAlign_Center)
				[
					SNew(STextBlock)
					.Font(ButtonTextStyle)
					.Text(QuitText)
					.Justification(ETextJustify::Center)
				]
				
			]
		]
	];
	// clang-format on
}

FReply SPauseMenuWidget::OnResume() const
{
	if (OwnHUD.IsValid())
	{
		OwnHUD->TogglePauseMenu();
	}
	return FReply::Handled();
}

FReply SPauseMenuWidget::OnQuit() const
{
	if (OwnHUD.IsValid())
	{
		if (APlayerController* PC = OwnHUD->GetWorld()->GetFirstPlayerController())
		{
			PC->ConsoleCommand("quit");
		}
	}
	return FReply::Handled();
}

#undef LOCTEXT_NAMESPACE
