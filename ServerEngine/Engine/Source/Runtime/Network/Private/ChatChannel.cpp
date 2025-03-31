#include "ChatChannel.h"
#include "Engine/NetConnection.h"
#include "IpNetDriver.h"
#include "IpConnection.h"

void UChatChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetChatMessage<NMT_CTS_Chat>::Receive(Bunch))
	{
		FChatMessage& ChatMessage = (FChatMessage&)(Bunch);

		UIpNetDriver* NetDriver = dynamic_cast<UIpNetDriver*>(Connection->GetNetDriver());
		_ASSERT(NetDriver);

		{
			FString UserName = std::format(TEXT("{}:{}"), Connection->GetRemoteIP(), Connection->GetRemotePort());
			ZeroMemory(ChatMessage.UserName, sizeof(ChatMessage.UserName));
			std::wcsncpy(ChatMessage.UserName, UserName.c_str(), UserName.size());
		}

		OnCTS_Chat.Broadcast(static_cast<UIpConnection*>(Connection), ChatMessage);
	}
	else if (FNetChatMessage<NMT_STC_Chat>::Receive(Bunch))
	{
		FChatMessage& ChatMessage = (FChatMessage&)(Bunch);
		ReceivedMessageDelegate.Broadcast(ChatMessage);
	}
}
