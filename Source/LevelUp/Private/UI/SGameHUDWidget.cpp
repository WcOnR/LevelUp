// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/SGameHUDWidget.h"

#include "GameFramework/HUD.h"
#include "UI/MainGameHUD.h"
#include "UI/SWeaponInterfaceWidget.h"
#include "InventoryComponent.h"

void SGameHUDWidget::Construct(const FArguments& InArgs)
{
	OwnHUD = InArgs._OwnHUD;
	bCanSupportFocus = true;
	CrosshairBrush = InArgs._CrosshairImg;

	// clang-format off
	ChildSlot
	[
		SAssignNew(MainOverlay, SOverlay)
	];
	// clang-format on

	if (APlayerController* PlayerController = OwnHUD->GetWorld()->GetFirstPlayerController())
	{
		if (APawn* PlayerPawn = PlayerController->GetPawn())
		{
			if (UInventoryComponent* Inventory = PlayerPawn->FindComponentByClass<UInventoryComponent>())
			{
				Inventory->OnWeaponPickUp.AddRaw(this, &SGameHUDWidget::ShowWeaponInterface);
			}
		}
	}
}

void SGameHUDWidget::ShowWeaponInterface()
{
	if (WeaponInterface)
	{
		return;
	}
	// clang-format off
	MainOverlay->AddSlot()
	[
		SAssignNew(WeaponInterface, SWeaponInterfaceWidget)
		.OwnHUD(OwnHUD)
		.CrosshairImg(CrosshairBrush)
	];
	// clang-format on
}
