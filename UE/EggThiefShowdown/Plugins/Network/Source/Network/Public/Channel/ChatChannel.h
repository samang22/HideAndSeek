// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SendRecvBunchChannel.h"
#include "ChatChannel.generated.h"

template<uint8 MessageType> class FNetChatMessage {};

#define DEFINE_CHAT_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Chat, 1, Name, Index)

DEFINE_CHAT_CHANNEL_MESSAGE(CTS_Chat, 0);
DEFINE_CHAT_CHANNEL_MESSAGE(STC_Chat, 1);

#pragma pack(push, 1)
class FHaChatMessage : public FBunch
{
public:
    WIDECHAR UserName[25] = {};
    WIDECHAR Message[256] = {};
};
#pragma pack(pop)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnChatMessage, FString, UserName, FString, Message);

UCLASS()
class NETWORK_API UChatChannel : public USendRecvBunchChannel
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnChatMessage OnChatMessage;

public:
    /** Handle an incoming bunch. */
    virtual void ReceivedBunch(FBunch& Bunch) override;
};