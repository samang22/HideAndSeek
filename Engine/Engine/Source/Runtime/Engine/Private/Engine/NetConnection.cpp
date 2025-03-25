#include "Engine/NetConnection.h"
#include "Engine/ControlChannel.h"
#include "Engine/NetDriver.h"

bool UNetConnection::InitRemoteConnection(UNetDriver* InDriver, const FURL& InURL)
{
    return InitBase(InDriver, InURL);
}

void UNetConnection::HandleConnectionEstablished()
{
    // 임시로 5개정도 잡아 둡니다
    Channels.resize(5);

    Channels[0] = NewObject<UChannel>(this, UControlChannel::StaticClass(), TEXT("Control"));
    Channels[0]->Init(this, 0);
}

bool UNetConnection::IsServer() const
{
    return NetDriver->IsServer();
}

bool UNetConnection::InitBase(UNetDriver* InDriver, const FURL& InURL)
{
    NetDriver = InDriver;
    URL = InURL;
    SetConnectionState(EConnectionState::USOCK_Invalid);
    return true;
}
