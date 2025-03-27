// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SendRecvBunchChannel.h"
#include "EchoChannel.generated.h"

template<uint8 MessageType> class FNetEchoMessage {};

#define DEFINE_ECHO_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Echo, 1, Name, Index)
DEFINE_ECHO_CHANNEL_MESSAGE(Echo, 0);

#pragma pack(push, 1)
class FEchoMessage : public FBunch
{
public:
    WIDECHAR Message[256] = {};
};
#pragma pack(pop)

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnEchoMessage, FString, Message);

UCLASS()
class NETWORK_API UEchoChannel : public USendRecvBunchChannel
{
    GENERATED_BODY()

public:
    UPROPERTY(BlueprintAssignable)
    FOnEchoMessage OnEchoMessage;

public:
    /** Handle an incoming bunch. */
    virtual void ReceivedBunch(FBunch& Bunch) override;
};