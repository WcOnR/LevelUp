// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SWeaponInterfaceWidget.h"

#include "GameFramework/HUD.h"
#include "InventoryComponent.h"
#include "../TP_WeaponComponent.h"

SWeaponInterfaceWidget::SWeaponInterfaceWidget()
{
	AmmoInMag = 0;
	AmmoInBag = 0;
}

void SWeaponInterfaceWidget::Construct(const FArguments& InArgs)
{
	OwnHUD = InArgs._OwnHUD;
	bCanSupportFocus = true;
	CrosshairBrush = InArgs._CrosshairImg;

	const FMargin AmmoTextPadding = FMargin(20.f);
	FSlateFontInfo AmmoTextStyle = FCoreStyle::Get().GetFontStyle("EmbossedText");
	AmmoTextStyle.Size = 60.f;

	// clang-format off
	ChildSlot
	[
		SNew(SOverlay)
		+ SOverlay::Slot()
		.HAlign(HAlign_Center)
		.VAlign(VAlign_Center)
		[
			SNew(SBox)
			.HAlign(HAlign_Center)
			.VAlign(VAlign_Center)
			.Content()
			[
				SNew(SImage)
				.Image(&CrosshairBrush)
			]
		]
		+ SOverlay::Slot()
		.HAlign(HAlign_Right)
		.VAlign(VAlign_Bottom)
		.Padding(AmmoTextPadding)
		[
			SAssignNew(AmmoText, STextBlock)
			.Font(AmmoTextStyle)
			.Justification(ETextJustify::Right)
		]
	];
	// clang-format on

	if (APlayerController* PlayerController = OwnHUD->GetWorld()->GetFirstPlayerController())
	{
		if (APawn* PlayerPawn = PlayerController->GetPawn())
		{
			if (UInventoryComponent* Inventory = PlayerPawn->FindComponentByClass<UInventoryComponent>())
			{
				Inventory->OnAmountOfAmmoChanged.AddRaw(this, &SWeaponInterfaceWidget::UpdateAmmoInBag);
				UpdateAmmoInBag(Inventory->GetAmountOfAmmo());
				if (UTP_WeaponComponent* Weapon = Inventory->GetRifle()->FindComponentByClass<UTP_WeaponComponent>())
				{
					Weapon->OnAmountOfAmmoChanged.AddRaw(this, &SWeaponInterfaceWidget::UpdateAmmoInMag);
					UpdateAmmoInMag(Weapon->GetCurrentAmmoInMag());
				}
			}
		}
	}
}

void SWeaponInterfaceWidget::UpdateAmmoInMag(int32 CurrentAmmo)
{
	AmmoInMag = CurrentAmmo;
	UpdateAmmoText();
}

void SWeaponInterfaceWidget::UpdateAmmoInBag(int32 CurrentAmmo)
{
	AmmoInBag = CurrentAmmo;
	UpdateAmmoText();
}

void SWeaponInterfaceWidget::UpdateAmmoText()
{
	if (!AmmoText)
	{
		return;
	}
	AmmoText->SetText(FText::FromString(FString::Printf(TEXT("%d/%d"), AmmoInMag, AmmoInBag)));
}
