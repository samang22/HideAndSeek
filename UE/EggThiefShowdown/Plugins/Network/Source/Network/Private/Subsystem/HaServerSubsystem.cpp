// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HaServerSubsystem.h"
#include "HaNetDriver.h"
#include "EchoChannel.h"

void UHaServerSubsystem::ConnectToServer(FStringView InHost, int32 InPort)
{
    NetDriver = NewObject<UHaNetDriver>(this, TEXT("Ha NetDriver"));
    FURL URL;
    URL.Host = InHost.IsEmpty() ? Host : InHost;
    URL.Port = InPort == -1 ? Port : InPort;
    FString OutError;
    if (!NetDriver->InitConnect(this, URL, OutError))
    {
        UE_LOG(LogTemp, Error, TEXT("Connect Failed"));
        RequestEngineExit(TEXT("Connect Failed"));
        return;
    }

    FString Message = TEXT("HelloUE!");
    UEchoChannel* EchoChannel = Cast<UEchoChannel>(NetDriver->ServerConnection->Channels[1]);
    FEchoMessage EchoMessage;
    std::wcsncpy(EchoMessage.Message, &Message[0], Message.Len());
    FNetEchoMessage<NMT_Echo>::Send(NetDriver->ServerConnection, EchoMessage);
}

void UHaServerSubsystem::Tick(float DeltaSeconds)
{
    if (!NetDriver)
    {
        return;
    }

    NetDriver->TickDispatch(DeltaSeconds);
}

bool UHaServerSubsystem::IsTickable() const
{
    return true; // 항상 틱 가능하도록 설정
}

TStatId UHaServerSubsystem::GetStatId() const
{
    // 통계 ID를 반환합니다.
    RETURN_QUICK_DECLARE_CYCLE_STAT(UHaServerSubsystem, STATGROUP_Tickables);
}