// Copyright Epic Games, Inc. All Rights Reserved.

#include "LevelUpGameMode.h"

#include "LevelUpCharacter.h"
#include "UObject/ConstructorHelpers.h"

#include "InventoryComponent.h"

ALevelUpGameMode::ALevelUpGameMode() : Super()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(
		TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
}

void ALevelUpGameMode::AddAmmo() const
{
#if !(UE_BUILD_SHIPPING || UE_BUILD_TEST)
	if (!GetWorld())
	{
		return;
	}
	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		if (APawn* Pawn = PlayerController->GetPawn())
		{
			if (UInventoryComponent* Inventory = Pawn->FindComponentByClass<UInventoryComponent>())
			{
				Inventory->AddAmmo(CheatAddAmmoAmount);
			}
		}
	}
#endif
}
