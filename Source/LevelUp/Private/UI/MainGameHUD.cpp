// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/MainGameHUD.h"
#include "Kismet/GameplayStatics.h"
#include "UI/SPauseMenuWidget.h"
#include "UI/SGameHUDWidget.h"
#include "Widgets/SWeakWidget.h"

AMainGameHUD::AMainGameHUD()
{
	bPauseMenuHidden = true;
}

void AMainGameHUD::TogglePauseMenu()
{
	UGameViewportClient* ViewpotrClient = GetWorld()->GetGameViewport();
	if (bPauseMenuHidden)
	{
		ViewpotrClient->RemoveViewportWidgetContent(WidgetContainer.ToSharedRef());
		TSharedRef<SWidget> MainWidget = SAssignNew(WidgetContainer, SWeakWidget).PossiblyNullContent(PauseMenu.ToSharedRef());
		ViewpotrClient->AddViewportWidgetContent(MainWidget);

		if (IsValid(PlayerOwner))
		{
			PlayerOwner->bShowMouseCursor = true;
			PlayerOwner->SetInputMode(FInputModeUIOnly());
		}
	}
	else
	{
		ViewpotrClient->RemoveViewportWidgetContent(WidgetContainer.ToSharedRef());
		TSharedRef<SWidget> MainWidget = SAssignNew(WidgetContainer, SWeakWidget).PossiblyNullContent(GameHUD.ToSharedRef());
		ViewpotrClient->AddViewportWidgetContent(MainWidget);

		if (IsValid(PlayerOwner))
		{
			PlayerOwner->bShowMouseCursor = false;
			PlayerOwner->SetInputMode(FInputModeGameOnly());
		}
	}
	UGameplayStatics::SetGamePaused(GetWorld(), bPauseMenuHidden);
	bPauseMenuHidden = !bPauseMenuHidden;
}

void AMainGameHUD::BeginPlay()
{
	Super::BeginPlay();
	FSlateBrush CrosshairBrush;
	if (IsValid(CrosshairImg))
	{
		CrosshairBrush.SetResourceObject(CrosshairImg);
		CrosshairImg->bForceMiplevelsToBeResident = true;
		CrosshairImg->bIgnoreStreamingMipBias = true;

		CrosshairBrush.ImageSize.X = CrosshairImg->GetSizeX();
		CrosshairBrush.ImageSize.Y = CrosshairImg->GetSizeY();
	}
	PauseMenu = SNew(SPauseMenuWidget).OwnHUD(this);
	GameHUD = SNew(SGameHUDWidget).OwnHUD(this).CrosshairImg(CrosshairBrush);

	UGameViewportClient* ViewpotrClient = GetWorld()->GetGameViewport();

	TSharedRef<SWidget> MainWidget = SAssignNew(WidgetContainer, SWeakWidget).PossiblyNullContent(GameHUD.ToSharedRef());
	ViewpotrClient->AddViewportWidgetContent(MainWidget);
}
