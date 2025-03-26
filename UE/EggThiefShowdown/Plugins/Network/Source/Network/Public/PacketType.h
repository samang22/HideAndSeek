#pragma once
#include "CoreMinimal.h"

#pragma pack(push, 1)
class FPacketHeader
{
public:
    // FNetControlMessage<>::Send 내부에서 채워집니다
    // Header를 포함 한 전체 packet size
    int32 PacketSize = 0;
};

class FBunch : public FPacketHeader /*: public FNetBitReader*/
{
public:
    int32 ChannelIndex = 0;
    int32 MessageType = 0;
};
#pragma pack(pop)

class USendRecvBunchChannel;

#define DEFINE_CHANNEL_MESSAGE(ChannelName, _ChannelIndex, Name, Index) \
enum { NMT_##Name = Index }; \
template<> class FNet##ChannelName##Message<Index> \
{ \
public: \
    /** 지정된 연결의 제어 채널로 이 유형의 메시지를 보냅니다. \
    * @note: const not used only because of the FArchive interface; the parameters are not modified \
    */ \
    template<typename NewBunch> \
    static void Send(UNetConnection* Conn, NewBunch& Bunch) \
    { \
        if (Conn->Channels[_ChannelIndex] != NULL) \
        { \
            Bunch.ChannelIndex = _ChannelIndex; \
            Bunch.MessageType = Index; \
            Bunch.PacketSize = sizeof(NewBunch); \
            Cast<USendRecvBunchChannel>(Conn->Channels[_ChannelIndex])->SendBunch(Bunch); \
        } \
    } \
    /** receives a message of this type from the passed in bunch */ \
    template<typename NewBunch>\
    [[nodiscard]] static bool Receive(NewBunch& Bunch) \
    { \
        return Bunch.MessageType == Index; \
    } \
};