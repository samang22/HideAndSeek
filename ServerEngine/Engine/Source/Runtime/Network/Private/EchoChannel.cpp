#include "EchoChannel.h"
#include "Engine/NetConnection.h"

void UEchoChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetEchoMessage<NMT_Echo>::Receive(Bunch))
	{
		FEchoMessage& EchoMessage = (FEchoMessage&)(Bunch);
		FString Msg = EchoMessage.Message;
		E_LOG(Log, TEXT("[{}:{}]: {}"), Connection->GetRemoteIP(), Connection->GetRemotePort(), Msg);

		if (Connection->IsServer())
		{
			FNetEchoMessage<NMT_Echo>::Send(Connection, EchoMessage);
		}
		ReceivedMessageDelegate.Broadcast(EchoMessage);
	}
}
