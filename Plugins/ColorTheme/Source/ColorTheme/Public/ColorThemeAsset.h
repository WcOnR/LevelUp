#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "ColorThemeAsset.generated.h"

UCLASS(BlueprintType)
class COLORTHEME_API UColorThemeAsset : public UObject
{
	GENERATED_BODY()
public:
	UColorThemeAsset() {}

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = Color)
	FLinearColor Color;

};