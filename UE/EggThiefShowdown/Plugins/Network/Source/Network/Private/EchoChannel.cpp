// Fill out your copyright notice in the Description page of Project Settings.


#include "EchoChannel.h"
#include "HaConnection.h"
#include "Net/Common/Packets/PacketTraits.h"

void UEchoChannel::Tick()
{
    Super::Tick();
}

void UEchoChannel::SendBunch(FBunch& NewBunch)
{
    FOutPacketTraits OutPacketTraits;
    Connection->LowLevelSend((void*)&NewBunch, NewBunch.PacketSize, OutPacketTraits);
}