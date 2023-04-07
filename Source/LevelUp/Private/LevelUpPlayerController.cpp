// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelUpPlayerController.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "UI/MainGameHUD.h"

void ALevelUpPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(InputComponent))
	{
		// Pause
		EnhancedInputComponent->BindAction(PauseAction, ETriggerEvent::Triggered, this, &ALevelUpPlayerController::OnPauseAction);
	}
}

void ALevelUpPlayerController::OnPauseAction()
{
	if (AMainGameHUD* Hud = Cast<AMainGameHUD>(GetHUD()))
	{
		Hud->TogglePauseMenu();
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
