// Fill out your copyright notice in the Description page of Project Settings.
#pragma once

#include "CoreMinimal.h"
#include "Engine/Channel.h"
#include "PacketType.h"
#include "SendBunchChannel.generated.h"

/**
 *
 */
UCLASS()
class NETWORK_API USendBunchChannel : public UChannel
{
    GENERATED_BODY()

public:
    virtual void SendBunch(FBunch& NewBunch) {}
};