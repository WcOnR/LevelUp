// Fill out your copyright notice in the Description page of Project Settings.

#if (WITH_DEV_AUTOMATION_TESTS || WITH_PREF_AUTOMATION_TESTS)

#include "CoreMinimal.h"
#include "Misc/AutomationTest.h"
#include "Kismet2/KismetEditorUtilities.h"

IMPLEMENT_SIMPLE_AUTOMATION_TEST(FBlueprintTest, "Code.Blueprint", EAutomationTestFlags::ApplicationContextMask | EAutomationTestFlags::SmokeFilter | EAutomationTestFlags::HighPriority);

bool FBlueprintTest::RunTest(const FString& Parameters)
{
	FMessageLog BlueprintLog("BlueprintLog");
	TArray<UBlueprint*> ErroredBlueprints;
	TArray<UBlueprint*> CompiledBlueprints;
	auto OnBlueprintPreCompileLambda = [&CompiledBlueprints](UBlueprint* InBlueprint)
	{
		check(InBlueprint != nullptr);
		CompiledBlueprints.Add(InBlueprint);
	};

	if (!GEditor)
	{
		return false;
	}

	AddInfo("Build blueprints");
	FDelegateHandle PreCompileDelegateHandle = GEditor->OnBlueprintPreCompile().AddLambda(OnBlueprintPreCompileLambda);
	for (TObjectIterator<UBlueprint> BlueprintIt; BlueprintIt; ++BlueprintIt)
	{
		UBlueprint* Blueprint = *BlueprintIt;
		FKismetEditorUtilities::CompileBlueprint(Blueprint,
			EBlueprintCompileOptions::SkipGarbageCollection | EBlueprintCompileOptions::SkipSave);

		TestTrue(FString::Printf(TEXT("%s actually compiles"), *Blueprint->GetName()), CompiledBlueprints.Num() > 0);
		for (UBlueprint* CompiledBlueprint : CompiledBlueprints)
		{
			const bool bHadError = (!CompiledBlueprint->IsUpToDate() && CompiledBlueprint->Status != BS_Unknown);
			if (ErroredBlueprints.Find(CompiledBlueprint) == INDEX_NONE)
			{
				ErroredBlueprints.Add(CompiledBlueprint);
				TestTrue(FString::Printf(TEXT("%s compiles without errors"), *CompiledBlueprint->GetName()), !bHadError);
			}
		}
		CompiledBlueprints.Empty();
	}
	return true;
}

#endif
