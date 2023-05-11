// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "MenuPlayerController.generated.h"

class UStartupMenuWidget;

UCLASS()
class LEVELUP_API AMenuPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void BeginPlay() override;

	UFUNCTION(Exec)
	void Host(const FString& ServerName);

	UFUNCTION(Exec)
	void ResetSession();

	UFUNCTION(Exec)
	void FindSession();

	UFUNCTION(Exec)
	void Join(int32 Index);

protected:
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = UI)
	TSubclassOf<UStartupMenuWidget> StartupMenuWidgetClass;

	UPROPERTY()
	UUserWidget* StartupMenuWidget;
};
