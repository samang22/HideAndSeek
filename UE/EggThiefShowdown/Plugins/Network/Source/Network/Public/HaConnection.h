// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IpConnection.h"
#include "Containers/CircularQueue.h"
#include "HaConnection.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_API UHaConnection : public UIpConnection
{
	GENERATED_BODY()

	friend class UHaNetDriver;

public:
	UHaConnection();
	// To use TCP not UDP, we override this class
	virtual void LowLevelSend(void* Data, int32 CountBits, FOutPacketTraits& Traits) override;

	virtual void OnConnect(bool bInNetDriverRecvThread);

	virtual class FBunch* GetPacket();

	virtual void TickDispatch(float DeltaTime);

protected:
	bool ReadPacketSome(const uint32 InReadSize);
	bool ReadPacket();

    /** Represents a packet received and/or error encountered by the receive thread, if enabled, queued for the game thread to process. */
    struct FHaReceivedPacket
    {
        /** The content of the packet as received from the socket. */
        TArray<uint8> PacketBytes;

        /** The error triggered by the socket RecvFrom call. */
        ESocketErrors Error;

        /** FPlatformTime::Seconds() at which this packet and/or error was received. Can be used for more accurate ping calculations. */
        double PlatformTimeSeconds;

        FHaReceivedPacket()
            : Error(SE_NO_ERROR)
            , PlatformTimeSeconds(0.0)
        {
        }
    };

    /** Thread-safe queue of received packets. The Run() function is the producer, UIpNetDriver::TickDispatch on the game thread is the consumer. */
    TUniquePtr<TCircularQueue<FHaReceivedPacket>> ReceiveQueue;

    bool DispatchPacket(FHaReceivedPacket& IncomingPacket);
    bool ProcessPacket(FHaReceivedPacket& InPacket);

protected:
    FHaReceivedPacket RecvBuffer;
    int32 RecvPacketSize = 0;
    bool bNetDriverRecvThread = false;
};
