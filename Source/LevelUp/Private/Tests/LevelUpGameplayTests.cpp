// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PREF_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Tests/AutomationCommon.h"
#include "Engine/World.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Pawn.h"
#include "Kismet/GameplayStatics.h"
#include "BreakableActor.h"
#include "AbilitySystemComponent.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FShootTest, "Gameplay.Shoot", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::ProductFilter | EAutomationTestFlags::HighPriority);

UWorld* GetTestGameWorld()
{
	const TIndirectArray<FWorldContext>& WorldContexts = GEngine->GetWorldContexts();
	for (const FWorldContext& Context : WorldContexts)
	{
		if ((Context.WorldType == EWorldType::PIE || Context.WorldType == EWorldType::Game) && Context.World())
		{
			return Context.World();
		}
	}

	return nullptr;
}

class MapScope
{
public:
	MapScope(const FString& MapPath) { AutomationOpenMap(MapPath, true); }
	~MapScope() { ADD_LATENT_AUTOMATION_COMMAND(FExitGameCommand); }
};

bool FShootTest::RunTest(const FString& Parameters)
{
	MapScope Scope("/Game/FirstPerson/Maps/TESTMap");
	UWorld* World = GetTestGameWorld();
	if (!TestNotNull("World exist", World))
	{
		return false;
	}
	APlayerController* PC = World->GetFirstPlayerController();
	if (!TestNotNull("PlayerController exist", PC))
	{
		return false;
	}
	IAbilitySystemInterface* ASI = Cast<IAbilitySystemInterface>(PC->GetPawn());
	if (!TestNotNull("Pawn has ASI exist", ASI))
	{
		return false;
	}
	UAbilitySystemComponent* ASC = ASI->GetAbilitySystemComponent();
	if (!TestNotNull("Pawn has ASC exist", ASC))
	{
		return false;
	}

	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(World, ABreakableActor::StaticClass(), Actors);
	int32 BoxNum = Actors.Num();
	if (!TestTrue("Box exist", BoxNum == 1))
	{
		return false;
	}

	auto MakeShoot = [=]()
	{
		ASC->AbilityLocalInputPressed(2); // 2 is id of shoot ability
	};
	auto CheckIsBoxDelete = [=]()
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(World, ABreakableActor::StaticClass(), Actors);
		if (Actors.Num() != 0)
		{
			UE_LOG(LogTemp, Error, TEXT("Box wasn't delete"));
		}
	};

	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(MakeShoot, 0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(MakeShoot, 0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(MakeShoot, 0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(MakeShoot, 0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FWaitLatentCommand(0.5f));
	ADD_LATENT_AUTOMATION_COMMAND(FDelayedFunctionLatentCommand(CheckIsBoxDelete));
	return true;
}

#endif
