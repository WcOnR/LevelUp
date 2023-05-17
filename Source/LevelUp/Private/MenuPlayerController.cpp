// Fill out your copyright notice in the Description page of Project Settings.


#include "MenuPlayerController.h"
#include "UI/StartupMenuWidget.h"
#include "OnlineManagerSubsystem.h"

void AMenuPlayerController::BeginPlay()
{
	Super::BeginPlay();
	if (IsValid(StartupMenuWidgetClass))
	{
		StartupMenuWidget = CreateWidget(GetWorld(), StartupMenuWidgetClass);
		UGameViewportClient* ViewpotrClient = GetWorld()->GetGameViewport();
		ViewpotrClient->AddViewportWidgetContent(StartupMenuWidget->TakeWidget());
	}
}

void AMenuPlayerController::Host(const FString& ServerName)
{
	check(GetGameInstance() && GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>());
	GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>()->Host(ServerName);
}

void AMenuPlayerController::ResetSession()
{
	check(GetGameInstance() && GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>());
	GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>()->ResetSession();
}

void AMenuPlayerController::FindSession()
{
	check(GetGameInstance() && GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>());
	GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>()->FindSession();
}

void AMenuPlayerController::Join(int32 Index)
{
	check(GetGameInstance() && GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>());
	GetGameInstance()->GetSubsystem<UOnlineManagerSubsystem>()->Join(Index);
}

void AMenuPlayerController::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (UGameViewportClient* ViewpotrClient = GetWorld()->GetGameViewport())
	{
		ViewpotrClient->RemoveViewportWidgetContent(StartupMenuWidget->TakeWidget());
	}
}
