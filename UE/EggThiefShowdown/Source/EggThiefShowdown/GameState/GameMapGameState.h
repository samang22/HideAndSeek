// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameState.h"
#include "GameMapGameState.generated.h"

/**
 *
 */
UCLASS()
class EGGTHIEFSHOWDOWN_API AGameMapGameState : public AGameState
{
    GENERATED_BODY()

public:
    AGameMapGameState();

protected:
    // EggGauge 값 (0.0 ~ 1.0 범위)
    UPROPERTY(ReplicatedUsing = OnRep_EggGauge)
    float EggGauge;
    UPROPERTY(ReplicatedUsing = OnRep_EggGauge)
    int32 EggCount;

public:
    // EggGauge 값을 설정
    void SetEggGauge(float NewEggGauge);

    // EggGauge 값을 가져옴
    float GetEggGauge() const { return EggGauge; }

protected:
    // EggGauge 값이 복제될 때 호출
    UFUNCTION()
    void OnRep_EggGauge();

    virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};