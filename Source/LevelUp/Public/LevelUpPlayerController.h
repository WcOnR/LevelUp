// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "LevelUpPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class LEVELUP_API ALevelUpPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void SetupInputComponent() override;

protected:
	void OnPauseAction();

protected:
	/** Pause Input Action */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* PauseAction;
};
