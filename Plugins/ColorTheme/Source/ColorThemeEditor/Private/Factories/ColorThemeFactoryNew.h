#pragma once

#include "Factories/Factory.h"
#include "UObject/ObjectMacros.h"

#include "ColorThemeFactoryNew.generated.h"


UCLASS()
class UColorThemeFactoryNew : public UFactory
{
	GENERATED_UCLASS_BODY()

public:
	virtual UObject* FactoryCreateNew(UClass* InClass, UObject* InParent, FName InName, EObjectFlags Flags, UObject* Context, FFeedbackContext* Warn) override;
};
