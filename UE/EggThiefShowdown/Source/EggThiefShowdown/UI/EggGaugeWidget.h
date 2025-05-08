// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EggGaugeWidget.generated.h"

class UProgressBar;
UCLASS()
class EGGTHIEFSHOWDOWN_API UEggGaugeWidget : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;
    virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

protected:
    UPROPERTY(meta = (BindWidget))
    TObjectPtr<UProgressBar> EggGaugeBar;
public:
    void SetEggGaugeBarPercent(float InPercent);
};