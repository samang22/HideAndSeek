// Fill out your copyright notice in the Description page of Project Settings.


#include "GameState/GameMapGameState.h"
#include "Net/UnrealNetwork.h"

AGameMapGameState::AGameMapGameState()
{
    // 복제 활성화
    bReplicates = true;
}
void AGameMapGameState::SetEggGauge(float NewEggGauge)
{
    if (HasAuthority()) // 서버에서만 값 설정 가능
    {
        EggGauge = FMath::Clamp(NewEggGauge, 0.0f, 1.0f); // 0.0 ~ 1.0 범위로 제한
        OnRep_EggGauge(); // 서버에서 직접 호출
    }
}

void AGameMapGameState::OnRep_EggGauge()
{
    // 클라이언트에서 EggGauge 값이 변경되었을 때 처리
    UE_LOG(LogTemp, Warning, TEXT("EggGauge updated: %f"), EggGauge);
}

void AGameMapGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
    Super::GetLifetimeReplicatedProps(OutLifetimeProps);

    DOREPLIFETIME(ThisClass, EggGauge);
    DOREPLIFETIME(ThisClass, EggCount);
}