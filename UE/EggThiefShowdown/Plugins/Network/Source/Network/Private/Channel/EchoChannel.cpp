// Fill out your copyright notice in the Description page of Project Settings.


#include "Channel/EchoChannel.h"
#include "HaConnection.h"
#include "Net/Common/Packets/PacketTraits.h"

void UEchoChannel::ReceivedBunch(FBunch& Bunch)
{
    if (FNetEchoMessage<NMT_Echo>::Receive(Bunch))
    {
        FEchoMessage& EchoMessage = (FEchoMessage&)Bunch;
        const FString Message = EchoMessage.Message;
        OnEchoMessage.Broadcast(Message);
    }
}