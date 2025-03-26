// Fill out your copyright notice in the Description page of Project Settings.


#include "HaConnection.h"
#include "Sockets.h"
#include "Net/Common/Packets/PacketTraits.h"

UHaConnection::UHaConnection()
{
    // 부모에서 안쓰는 기능으므로 비활성화
    DisableAddressResolution();
}

void UHaConnection::LowLevelSend(void* Data, int32 CountBits, FOutPacketTraits& Traits)
{
    if (CountBits == 0 || Data == nullptr)
    {
        UE_LOG(LogNet, Warning, TEXT("CountBits == 0 || Data == nullptr"));
        check(false);
        return;
    }

    int32 SentBytes = 0;
    if (!Socket->Send((const uint8*)Data, CountBits, SentBytes))
    {
        UE_LOG(LogNet, Warning, TEXT("UARNetConnection: LowLevelSend: Server로 Data를 보낼 수 없습니다. %d"), CountBits);
        check(false);
    }

    if (SentBytes != CountBits)
    {
        UE_LOG(LogNet, Warning, TEXT("UARNetConnection: LowLevelSend: SentBytes(%d) != CountBits(%d)"), SentBytes, CountBits);
        check(false);
        /*UARNetDriver* ARNetDriver = Cast<UARNetDriver>(Driver);
        ARNetDriver->Shutdown();*/
        return;
    }

    // 서버 Recv buffer가 가득 참(너무 빨리 Packet을 보냄)
    if (SentBytes == -1)
    {
        UE_LOG(LogNet, Warning, TEXT("UARNetConnection: LowLevelSend: SentBytes == -1"));
        check(false);
        /*UARNetDriver* ARNetDriver = Cast<UARNetDriver>(Driver);
        ARNetDriver->Shutdown();*/
        return;
    }
}