// Fill out your copyright notice in the Description page of Project Settings.


#include "Channel/LoginChannel.h"

void ULoginChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetLoginMessage<NMT_STC_CreateAccountResult>::Receive(Bunch))
	{
		FHaCreateAccountResult& HaCreateAccountResult = (FHaCreateAccountResult&)Bunch;
		OnCreateAccountResult.Broadcast(HaCreateAccountResult.ResultCode);
	}
	else if (FNetLoginMessage<NMT_STC_LoginResult>::Receive(Bunch))
	{
		FHaLoginResult& HaLoginResult = (FHaLoginResult&)Bunch;
		OnLoginResult.Broadcast(HaLoginResult.ResultCode);

		if (HaLoginResult.ResultCode)
		{
			LoginUserName = HaLoginResult.UserName;
			LoginPassword = HaLoginResult.Password;
		}
	}
	else if (FNetLoginMessage<NMT_STC_DediServerInfo>::Receive(Bunch))
	{
		FHaDediServerInfo& HaDediServerInfo = (FHaDediServerInfo&)Bunch;
		ServerCount = HaDediServerInfo.ServerCount;
		DediServerInfos.SetNum(ServerCount);
		for (int32 i = 0; i < ServerCount; ++i)
		{
			DediServerInfos[i].MaxPlayers = HaDediServerInfo.MaxPlayers[i];
			DediServerInfos[i].CurrentPlayers = HaDediServerInfo.CurrentPlayers[i];
			DediServerInfos[i].IP = FString(ANSI_TO_TCHAR(HaDediServerInfo.ServerIP[i]));
			DediServerInfos[i].Port = HaDediServerInfo.Ports[i];
		}
	}
}
