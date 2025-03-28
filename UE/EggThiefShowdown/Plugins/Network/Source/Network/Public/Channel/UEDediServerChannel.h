// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Channel/SendRecvBunchChannel.h"
#include "UEDediServerChannel.generated.h"

template<uint8 MessageType> class FNetDediServerChannelMessage {};

#define DEFINE_UEDEDI_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(DediServerChannel, 4, Name, Index)

#pragma pack(push, 1)
DEFINE_UEDEDI_CHANNEL_MESSAGE(UETS_Info, 0);
class FHaDediServerChannelInfo : public FBunch
{
public:
	uint16 MaxPlayers;
	uint16 CurrentPlayers;
	uint16 Port;
};

DEFINE_UEDEDI_CHANNEL_MESSAGE(UETS_CheckAccountValid, 1);
class FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValid : public FBunch
{
public:
	WIDECHAR UserName[15] = {};
	WIDECHAR Password[15] = {};
};

DEFINE_UEDEDI_CHANNEL_MESSAGE(STUE_CheckAccountValid, 2);
class FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult : public FBunch
{
public:
	bool bResult;
	WIDECHAR UserName[15] = {};
	WIDECHAR Password[15] = {};
};

// 데디 서버에서 해당 유저가 있는 경우 킥합니다
DEFINE_UEDEDI_CHANNEL_MESSAGE(STUE_KickUser, 3);
class FHa_DEDI_TO_LOGIN_SERVER_KickUser : public FBunch
{
public:
	WIDECHAR UserName[15] = {};
};
#pragma pack(pop)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnCheckAccountValidResult, bool, bResult, FString, UserName, FString, Password);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnKickPlayer, FString, UserName);

UCLASS()
class NETWORK_API UUEDediServerChannel : public USendRecvBunchChannel
{
	GENERATED_BODY()

public:
	/** Handle an incoming bunch. */
	virtual void ReceivedBunch(FBunch& Bunch) override;

	FOnCheckAccountValidResult OnCheckAccountValidResult;
	FOnKickPlayer OnKickPlayer;
};
