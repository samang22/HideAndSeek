#include "Engine/ControlChannel.h"
#include "Engine/NetConnection.h"
#include "Net/DataChannel.h"

void UControlChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetControlMessage<NMT_Hello>::Receive(Bunch))
	{
		int a = 10;
	}
}
