// Fill out your copyright notice in the Description page of Project Settings.


#include "HaConnection.h"
#include "Sockets.h"
#include "Net/Common/Packets/PacketTraits.h"
#include "PacketType.h"
#include "SendRecvBunchChannel.h"
#include "SocketSubsystem.h"

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

void UHaConnection::OnConnect()
{
    RecvBuffer.SetNumUninitialized(4096, false);
}

FBunch* UHaConnection::GetPacket()
{
    FBunch* Bunch = (FBunch*)RecvBuffer.GetData();
    return Bunch;
}

bool UHaConnection::ReadPacketSome(const uint32 InReadSize)
{
    if (InReadSize == 0)
    {
        return false;
    }
    const int32 MaxRecvSize = RecvPacketSize + InReadSize;
    if (MaxRecvSize > RecvBuffer.Num())
    {
        UE_LOG(LogNet, Error, TEXT("MaxRecvSize > RecvBuffer.Num(): %d > %d"), MaxRecvSize, RecvBuffer.Num());
        // Shutdown()
        return false;
    }

    int32 BytesRead = 0;
    if (Socket->Recv(&RecvBuffer[RecvPacketSize], InReadSize, BytesRead))
    {
        RecvPacketSize += BytesRead;

        ISocketSubsystem* SocketSubsystem = ISocketSubsystem::Get();
        ESocketErrors Error = SocketSubsystem->GetLastErrorCode();

        if (BytesRead == 0)
        {
            // NonBlock Recv를 호출 했으나, 네트워크 버퍼에 읽을 데이터가 없다
            if (Error == SE_EWOULDBLOCK)
            {
                return false;
            }
            else
            {
                check(false);
            }
        }

        return true;
    }
    else if (BytesRead == 0)
    {
        // 서버가 끊어짐
        // Shutdown();
        return false;
    }

    check(false);
    // Shutdown();

    return false;
}

bool UHaConnection::ReadPacket()
{
    const bool bHeader = RecvPacketSize >= sizeof(FBunch);
    if (bHeader)
    {
        FBunch* Bunch = (FBunch*)RecvBuffer.GetData();
        const int32 PacketSize = Bunch->PacketSize;
        const int32 ReadPacketSize = PacketSize - RecvPacketSize;
        const bool bRead = ReadPacketSome(ReadPacketSize);

        if ((bRead || ReadPacketSize == 0) && RecvPacketSize >= PacketSize)
        {
            // 한 Packet이 완성 됨
            RecvPacketSize = 0;

            const int32 ChannelIndex = Bunch->ChannelIndex;

            if (ChannelIndex < Channels.Num() && Channels[ChannelIndex])
            {
                USendRecvBunchChannel* Channel = Cast<USendRecvBunchChannel>(Channels[ChannelIndex]);
                if (Channel)
                {
                    Channel->ReceivedBunch(*Bunch);
                    return true;
                }
                else
                {
                    check(false);
                }
            }
            check(false);
        }
        return false;
    }
    else
    {
        const uint32 RemainHeaderSize = sizeof(FBunch) - RecvPacketSize;
        return ReadPacketSome(RemainHeaderSize);
    }
}
