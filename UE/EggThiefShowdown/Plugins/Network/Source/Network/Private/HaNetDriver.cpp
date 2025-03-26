// Fill out your copyright notice in the Description page of Project Settings.


#include "HaNetDriver.h"
#include "SocketSubsystem.h"
#include "Sockets.h"
#include "PacketType.h"
#include "HaConnection.h"

bool UHaNetDriver::InitConnectionClass()
{
    NetConnectionClassName = TEXT("/Script/Network.HaConnection");
    return Super::InitConnectionClass();
}

bool UHaNetDriver::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
    bool bResult =  Super::InitConnect(InNotify, ConnectURL, Error);

	if (!bResult)
	{
		return bResult;
	}

    HaServerConnection = Cast<UHaConnection>(ServerConnection);
    HaServerConnection->GetSocket()->SetNonBlocking(false);
    HaServerConnection->GetSocket()->SetNoDelay(true);

	if (HaServerConnection->GetSocket()->Connect(*HaServerConnection->RemoteAddr))
	{
        HaServerConnection->GetSocket()->SetNonBlocking(true);
        HaServerConnection->OnConnect();
		return true;
	}
	else
	{
        ServerConnection = nullptr;
        HaServerConnection = nullptr;
		return false;
	}

}

FUniqueSocket UHaNetDriver::CreateSocketForProtocol(const FName& ProtocolType)
{
	// Create TCP socket
	ISocketSubsystem* SocketSubsystem = GetSocketSubsystem();

	if (SocketSubsystem == NULL)
	{
		UE_LOG(LogNet, Warning, TEXT("UHaNetDriver::CreateSocket: Unable to find socket subsystem"));
		return NULL;
	}

	return SocketSubsystem->CreateUniqueSocket(NAME_Stream /*TCP*/, TEXT("Ha"), ProtocolType);
}

FUniqueSocket UHaNetDriver::CreateAndBindSocket(TSharedRef<FInternetAddr> BindAddr, int32 Port, bool bReuseAddressAndPort, int32 DesiredRecvSize, int32 DesiredSendSize, FString& Error)
{
    ISocketSubsystem* SocketSubsystem = GetSocketSubsystem();
    if (SocketSubsystem == nullptr)
    {
        Error = TEXT("Unable to find socket subsystem");
        return nullptr;
    }

    // Create the socket that we will use to communicate with
    FUniqueSocket NewSocket = CreateSocketForProtocol(BindAddr->GetProtocolType());

    if (!NewSocket.IsValid())
    {
        Error = FString::Printf(TEXT("%s: socket failed (%i)"), SocketSubsystem->GetSocketAPIName(), (int32)SocketSubsystem->GetLastErrorCode());
        return nullptr;
    }

    /* Make sure to cleanly destroy any sockets we do not mean to use. */
    ON_SCOPE_EXIT
    {
        if (Error.IsEmpty() == false)
        {
            NewSocket.Reset();
        }
    };

    //if (SocketSubsystem->RequiresChatDataBeSeparate() == false && NewSocket->SetBroadcast() == false)
    //{
    //    Error = FString::Printf(TEXT("%s: setsockopt SO_BROADCAST failed (%i)"), SocketSubsystem->GetSocketAPIName(), (int32)SocketSubsystem->GetLastErrorCode());
    //    return nullptr;
    //}

    if (NewSocket->SetReuseAddr(bReuseAddressAndPort) == false)
    {
        UE_LOG(LogNet, Log, TEXT("setsockopt with SO_REUSEADDR failed"));
    }

    if (NewSocket->SetRecvErr() == false)
    {
        UE_LOG(LogNet, Log, TEXT("setsockopt with IP_RECVERR failed"));
    }

    int32 ActualRecvSize(0);
    int32 ActualSendSize(0);
    NewSocket->SetReceiveBufferSize(DesiredRecvSize, ActualRecvSize);
    NewSocket->SetSendBufferSize(DesiredSendSize, ActualSendSize);
    UE_LOG(LogInit, Log, TEXT("%s: Socket queue. Rx: %i (config %i) Tx: %i (config %i)"), SocketSubsystem->GetSocketAPIName(),
        ActualRecvSize, DesiredRecvSize, ActualSendSize, DesiredSendSize);

    // Bind socket to our port.
    BindAddr->SetPort(Port);

    int32 AttemptPort = BindAddr->GetPort();
    int32 BoundPort = SocketSubsystem->BindNextPort(NewSocket.Get(), *BindAddr, MaxPortCountToTry + 1, 1);
    if (BoundPort == 0)
    {
        Error = FString::Printf(TEXT("%s: binding to port %i failed (%i)"), SocketSubsystem->GetSocketAPIName(), AttemptPort,
            (int32)SocketSubsystem->GetLastErrorCode());

        if (bExitOnBindFailure)
        {
            UE_LOG(LogNet, Fatal, TEXT("Fatal error: %s"), *Error);
        }

        return nullptr;
    }

    if (NewSocket->SetNonBlocking() == false)
    {
        Error = FString::Printf(TEXT("%s: SetNonBlocking failed (%i)"), SocketSubsystem->GetSocketAPIName(),
            (int32)SocketSubsystem->GetLastErrorCode());
        return nullptr;
    }

    return NewSocket;
}


void UHaNetDriver::TickDispatch(float DeltaTime)
{
    UNetDriver::TickDispatch(DeltaTime);
    //UIpNetDriver::TickDispatch(DeltaTime);

    if (!HaServerConnection) { return; }

    while (true)
    {
        if (!HaServerConnection->ReadPacket())
        {
            ISocketSubsystem* SocketSubsystem = GetSocketSubsystem();
            ESocketErrors Error = SocketSubsystem->GetLastErrorCode();

            // SE_ECONNRESET: 서버 접속이 끊어짐
            if (Error == SE_ECONNRESET || Error == SE_ENOTCONN)
            {
                Shutdown();
                return;
            }
            // NonBlock Recv를 호출 했으나, 네트워크 버퍼에 읽을 데이터가 없다
            else if (Error == SE_EWOULDBLOCK)
            {
                break;
            }
            /*const EConnectionState ConnectionState = ARServerConnection->GetConnectionState();
            if (ConnectionState == EConnectionState::USOCK_Closed || ConnectionState == EConnectionState::USOCK_Invalid)
            {
                return;
            }*/
        }
    }
}