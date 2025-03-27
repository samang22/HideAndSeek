
#include "Channel/SendRecvBunchChannel.h"
#include "HaConnection.h"
#include "Net/Common/Packets/PacketTraits.h"

void USendRecvBunchChannel::SendBunch(FBunch& NewBunch)
{
    FOutPacketTraits OutPacketTraits;
    Connection->LowLevelSend((void*)&NewBunch, NewBunch.PacketSize, OutPacketTraits);
}