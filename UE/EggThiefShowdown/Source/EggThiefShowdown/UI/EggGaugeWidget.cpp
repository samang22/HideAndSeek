// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/EggGaugeWidget.h"
#include "Components/ProgressBar.h"
#include "GameState/GameMapGameState.h"

void UEggGaugeWidget::NativeConstruct()
{
    Super::NativeConstruct();
    // ProgressBar 초기화
    if (EggGaugeBar)
    {
        EggGaugeBar->SetPercent(0.0f); // 초기값 설정
    }
}

void UEggGaugeWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
    Super::NativeTick(MyGeometry, InDeltaTime);

    // GameState에서 EggGauge 값을 가져옴
    if (APlayerController* PC = GetOwningPlayer())
    {
        if (AGameMapGameState* GameState = PC->GetWorld()->GetGameState<AGameMapGameState>())
        {
            const float CurrentEggGauge = GameState->GetEggGauge();
            SetEggGaugeBarPercent(CurrentEggGauge); // ProgressBar 업데이트
        }
    }
}

void UEggGaugeWidget::SetEggGaugeBarPercent(float InPercent)
{
    EggGaugeBar->SetPercent(InPercent);
}