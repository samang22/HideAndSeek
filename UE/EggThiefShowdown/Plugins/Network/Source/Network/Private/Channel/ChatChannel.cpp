// Fill out your copyright notice in the Description page of Project Settings.


#include "Channel/ChatChannel.h"
#include "HaConnection.h"
#include "Net/Common/Packets/PacketTraits.h"

void UChatChannel::ReceivedBunch(FBunch& Bunch)
{
    if (FNetChatMessage<NMT_STC_Chat>::Receive(Bunch))
    {
        FHaChatMessage& ChatMessage = (FHaChatMessage&)(Bunch);
        FString UserName = ChatMessage.UserName;
        FString Message = ChatMessage.Message;
        OnChatMessage.Broadcast(UserName, Message);
    }
}