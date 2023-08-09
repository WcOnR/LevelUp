// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "LevelUpSaveGame.h"
#include "Kismet/GameplayStatics.h"
#include "UI/MainGameHUD.h"

void ALevelUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ALevelUpPlayerController::OnPauseAction);
		EnhancedInputComponent->BindAction(SaveAction, ETriggerEvent::Triggered, this, &ALevelUpPlayerController::OnSaveGame);
		EnhancedInputComponent->BindAction(LoadAction, ETriggerEvent::Triggered, this, &ALevelUpPlayerController::OnLoadGame);
	}
}

void ALevelUpPlayerController::OnPauseAction()
{
	if (AMainGameHUD* Hud = Cast<AMainGameHUD>(GetHUD()))
	{
		Hud->TogglePauseMenu();
	}
}

void ALevelUpPlayerController::OnSaveGame()
{
	if (!HasAuthority() || !GetPawn())
		return;
	ULevelUpSaveGame* Save = nullptr;
	if (UGameplayStatics::DoesSaveGameExist("Quick", 0))
	{
		Save = Cast<ULevelUpSaveGame>(UGameplayStatics::LoadGameFromSlot("Quick", 0));
	}
	else
	{
		Save = Cast<ULevelUpSaveGame>(UGameplayStatics::CreateSaveGameObject(ULevelUpSaveGame::StaticClass()));
	}
	check(Save);
	Save->PlayerPosition = GetPawn()->GetActorLocation();
	Save->PlayerRotation = GetControlRotation();
	UGameplayStatics::SaveGameToSlot(Save, "Quick", 0);
	UE_LOG(LogTemp, Error, TEXT("Game Saved %s / %s"), *Save->PlayerPosition.ToString(), *Save->PlayerRotation.ToString());
}

void ALevelUpPlayerController::OnLoadGame()
{
	if (!HasAuthority() || !GetPawn())
		return;
	if (UGameplayStatics::DoesSaveGameExist("Quick", 0))
	{
		ULevelUpSaveGame* Save = Cast<ULevelUpSaveGame>(UGameplayStatics::LoadGameFromSlot("Quick", 0));
		check(Save);
		GetPawn()->SetActorLocation(Save->PlayerPosition);
		SetControlRotation(Save->PlayerRotation);
		UE_LOG(LogTemp, Error, TEXT("Game Loaded %s / %s"), *Save->PlayerPosition.ToString(), *Save->PlayerRotation.ToString());
	}
}

void ALevelUpPlayerController::AcknowledgePossession(APawn* P)
{
	Super::AcknowledgePossession(P);

	if (IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(P))
	{
		ASI->GetAbilitySystemComponent()->InitAbilityActorInfo(P, P);
	}
}
