// Fill out your copyright notice in the Description page of Project Settings.


#include "Channel/UEDediServerChannel.h"

void UUEDediServerChannel::ReceivedBunch(FBunch& Bunch)
{
	if (FNetDediServerChannelMessage<NMT_STUE_CheckAccountValid>::Receive(Bunch))
	{
		FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult& AR_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult = (FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult&)Bunch;
		FString UserName = AR_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult.UserName;
		FString Password = AR_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult.Password;
		OnCheckAccountValidResult.Broadcast(AR_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult.bResult, UserName, Password);
	}
	else if (FNetDediServerChannelMessage<NMT_STUE_KickUser>::Receive(Bunch))
	{
		FHa_DEDI_TO_LOGIN_SERVER_KickUser& AR_DEDI_TO_LOGIN_SERVER_KickUser = (FHa_DEDI_TO_LOGIN_SERVER_KickUser&)Bunch;
		FString UserName = AR_DEDI_TO_LOGIN_SERVER_KickUser.UserName;
		OnKickPlayer.Broadcast(UserName);
	}
}
