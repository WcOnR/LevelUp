#include "UI/TimerWidget.h"
#include "Components/TextBlock.h"

void UTimerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	if (!SecTimerHandle.IsValid())
	{
		SecLabel->SetText(FText::FromString(FString("00")));
		GetWorld()->GetTimerManager().SetTimer(SecTimerHandle, this, &UTimerWidget::OnSecTimer, 1.f, true);
	}
}

void UTimerWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	if (IsValid(MilliSecLabel))
	{
		int32 MilliSec = static_cast<int32>(GetWorld()->GetTimeSeconds() * 1000) % 1000;
		MilliSecLabel->SetText(FText::FromString(FString::Printf(TEXT("%03d"), MilliSec)));
	}
}

void UTimerWidget::OnSecTimer()
{
	SecLabel->SetText(FText::FromString(FString::Printf(TEXT("%02d"), static_cast<int32>(GetWorld()->GetTimeSeconds()))));
}
