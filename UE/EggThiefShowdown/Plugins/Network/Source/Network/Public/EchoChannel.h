// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SendBunchChannel.h"
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

/**
 *
 */
UCLASS()
class NETWORK_API UEchoChannel : public USendBunchChannel
{
    GENERATED_BODY()

public:
    virtual void Tick() override;

    virtual void SendBunch(FBunch& NewBunch);
};