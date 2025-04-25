// Fill out your copyright notice in the Description page of Project Settings.


#include "UI/HPStaminaWidget.h"
#include "Components/ProgressBar.h"

void UHPStaminaWidget::NativeConstruct()
{
}

void UHPStaminaWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
}

void UHPStaminaWidget::SetHPBarPercent(float InPercent)
{
	HPBar->SetPercent(InPercent);
}

void UHPStaminaWidget::SetStaminaBarPercent(float InPercent)
{
	StaminaBar->SetPercent(InPercent);
}
