#include "ChannelConnection.h"
#include "IpNetDriver.h"
#include "NetworkTypes.h"
#include "Engine/Channel.h"
#include "UEDediServerChannel.h"

void UChannelConnection::HandleConnectionEstablished()
{
	Super::HandleConnectionEstablished();

	if (Channels[4] != nullptr)
	{
		E_LOG(Error, TEXT("Channels not empty"));
	}

	Channels[4] = NewObject<UChannel>(this, UUEDediServerChannel::StaticClass(), TEXT("UEDediServerChannel"));
	Channels[4]->Init(this, 1);
}