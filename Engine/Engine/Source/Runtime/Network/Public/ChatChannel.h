#pragma once

#include "Engine/Channel.h"
#include "Net/DataBunch.h"
#include "ChatChannel.generated.h"

template<uint8 MessageType> class FNetChatMessage {};

#define DEFINE_CHAT_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Chat, 2, Name, Index)

DEFINE_CHAT_CHANNEL_MESSAGE(CTS_Chat, 0);
DEFINE_CHAT_CHANNEL_MESSAGE(STC_Chat, 1); // Broadcast

#pragma pack(push, 1)
class FChatMessage : public FBunch
{
public:
	WIDECHAR UserName[25] = {};
	WIDECHAR Message[256] = {};
};
#pragma pack(pop)

/**
 * 채팅 서버 및 클라를 위한 Sample Channel
 */
UCLASS()
class NETWORK_API UChatChannel : public UChannel
{
	GENERATED_BODY()

public:
	/** Handle an incoming bunch. */
	virtual void ReceivedBunch(FBunch& Bunch) override;

	FSafeDelegate<class UIpConnection*, FChatMessage&> OnCTS_Chat;
	FSafeDelegate<FChatMessage&> ReceivedMessageDelegate;
};