// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "MainGameHUD.generated.h"


class SPauseMenuWidget;
class SGameHUDWidget;
class SWidget;
class UTimerWidget;

UCLASS()
class LEVELUP_API AMainGameHUD : public AHUD
{
	GENERATED_BODY()
	
public:
	AMainGameHUD();

	void TogglePauseMenu();
	bool IsPauseMenuHidden() const { return bPauseMenuHidden; }
	
protected:
	void BeginPlay() override;

	UFUNCTION()
	void OnHUDDestroyed(AActor* DestroyedActor);

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	UTexture2D* CrosshairImg;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UTimerWidget> TimerWidgetClass;
	
	UPROPERTY()
	UUserWidget* TimerWidget;

	TSharedPtr<SPauseMenuWidget> PauseMenu;
	TSharedPtr<SGameHUDWidget> GameHUD;
	TSharedPtr<SWidget> WidgetContainer;

	bool bPauseMenuHidden;
};
