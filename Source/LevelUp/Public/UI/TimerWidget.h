#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TimerWidget.generated.h"

class UTextBlock;

UCLASS(Abstract)
class UTimerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;

	void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void OnSecTimer();

protected:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* SecLabel;
	UPROPERTY(BlueprintReadWrite, meta = (BindWidgetOptional))
	UTextBlock* MilliSecLabel;

	FTimerHandle SecTimerHandle;
};