#include "ChatChannel.h"
#include "Engine/NetConnection.h"
#include "IpNetDriver.h"
#include "IpConnection.h"

void UChatChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetChatMessage<NMT_CTS_Chat>::Receive(Bunch))
	{
		FChatMessage& ChatMessage = (FChatMessage&)(Bunch);
		//FString Msg = ChatMessage.Message;
		//E_LOG(Log, TEXT("[{}:{}]: {}"), Connection->GetRemoteIP(), Connection->GetRemotePort(), Msg);

		UIpNetDriver* NetDriver = dynamic_cast<UIpNetDriver*>(Connection->GetNetDriver());
		//_ASSERT(NetDriver);

		// IP와 Port로 User이름을 지정합니다
		{
			FString UserName = std::format(TEXT("{}:{}"), Connection->GetRemoteIP(), Connection->GetRemotePort());
			ZeroMemory(ChatMessage.UserName, sizeof(ChatMessage.UserName));
			std::wcsncpy(ChatMessage.UserName, UserName.c_str(), UserName.size());
		}
		map<UIpConnection*, TObjectPtr<UIpConnection>>& Connections = NetDriver->GetConnections();
		for (auto& It : Connections)
		{
			FNetChatMessage<NMT_STC_Chat>::Send((UNetConnection*)It.first, ChatMessage);
		}
		OnCTS_Chat.Broadcast(static_cast<UIpConnection*>(Connection), ChatMessage);
	}
	else if (FNetChatMessage<NMT_STC_Chat>::Receive(Bunch))
	{
		FChatMessage& ChatMessage = (FChatMessage&)(Bunch);
		ReceivedMessageDelegate.Broadcast(ChatMessage);
	}
}
