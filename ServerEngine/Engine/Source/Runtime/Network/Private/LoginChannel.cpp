#include "Engine/NetConnection.h"
#include "LoginChannel.h"
#include "IpConnection.h"

void ULoginChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetLoginMessage<NMT_CTS_CreateAccount>::Receive(Bunch))
	{
		FARLogin& Login = (FARLogin&)(Bunch);
		ReceivedDelegate.Broadcast(dynamic_cast<UIpConnection*>(Connection), Login);
	}
	else if (FNetLoginMessage<NMT_CTS_DediServerInfo>::Receive(Bunch))
	{
		RequestDediServerInfo.Broadcast(dynamic_cast<UIpConnection*>(Connection));
	}
}
