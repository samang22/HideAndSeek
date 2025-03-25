#include "Engine/Channel.h"
#include "Engine/NetConnection.h"
#include "Net/DataBunch.h"

void UChannel::Init(UNetConnection* InConnection, int32 InChIndex)
{
	Connection = InConnection;
	if (Connection)
	{
		NetDriver = Connection->GetNetDriver();
	}

	ChIndex = InChIndex;
}

void UChannel::SendBunch(FBunch& Bunch)
{
	if (Bunch.PacketSize == 0)
	{
		E_LOG(Error, TEXT("Wrong PacketSize"));
		return;
	}
	Connection->LowLevelSend((void*)&Bunch, Bunch.PacketSize);
}
