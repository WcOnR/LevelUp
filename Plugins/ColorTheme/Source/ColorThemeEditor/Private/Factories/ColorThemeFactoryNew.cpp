#include "ColorThemeFactoryNew.h"
#include "ColorThemeAsset.h"


UColorThemeFactoryNew::UColorThemeFactoryNew(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	SupportedClass = UColorThemeAsset::StaticClass();
	bCreateNew = true;
}

UObject* UColorThemeFactoryNew::FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn)
{
	return NewObject<UColorThemeAsset>(InParent, InClass, InName, Flags);
}