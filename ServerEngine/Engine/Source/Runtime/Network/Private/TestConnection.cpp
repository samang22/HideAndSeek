#include "TestConnection.h"
#include "IpNetDriver.h"
#include "NetworkTypes.h"
#include "Engine/Channel.h"
#include "EchoChannel.h"
#include "ChatChannel.h"
#include "LoginChannel.h"

void UTestConnection::HandleConnectionEstablished()
{
	Super::HandleConnectionEstablished();

	if (Channels[1] != nullptr || Channels[2] != nullptr || Channels[3] != nullptr)
	{
		E_LOG(Error, TEXT("Channels not empty"));
		return;
	}

	Channels[1] = NewObject<UChannel>(this, UEchoChannel::StaticClass(), TEXT("Echo"));
	Channels[1]->Init(this, 1);

	Channels[2] = NewObject<UChannel>(this, UChatChannel::StaticClass(), TEXT("Chat"));
	Channels[2]->Init(this, 2);

}