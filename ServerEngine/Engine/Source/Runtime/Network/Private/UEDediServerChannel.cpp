#include "UEDediServerChannel.h"
#include "Engine/NetConnection.h"
#include "IpNetDriver.h"
#include "IpConnection.h"
#include "UEDediServerChannel.h"

void UUEDediServerChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetDediServerChannelMessage<NMT_UETS_Info>::Receive(Bunch))
	{
		FHaDediServerChannelInfo& HaDediServerChannelInfo = (FHaDediServerChannelInfo&)Bunch;
		MaxPlayers = HaDediServerChannelInfo.MaxPlayers;
		CurrentPlayers = HaDediServerChannelInfo.CurrentPlayers;
		Port = HaDediServerChannelInfo.Port;
		E_LOG(Log, TEXT("IP: {}, MaxPlayers: {}, CurrentPlayers: {}, Port: {}"), Connection->GetRemoteIP(), MaxPlayers, CurrentPlayers, Port);
	}
	else if (FNetDediServerChannelMessage<NMT_UETS_CheckAccountValid>::Receive(Bunch))
	{
		FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValid& AR_DEDI_TO_LOGIN_SERVER_CheckAccountValid = (FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValid&)Bunch;
		RequestCheckAccountDelegate.Broadcast(dynamic_cast<UIpConnection*>(Connection), AR_DEDI_TO_LOGIN_SERVER_CheckAccountValid);
	}
}
