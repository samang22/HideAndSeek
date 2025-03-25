#pragma once

#include "Engine/Channel.h"
#include "Net/DataBunch.h"
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
 * 에코 서버 및 클라를 위한 Sample Channel
 */
UCLASS()
class NETWORK_API UEchoChannel : public UChannel
{
	GENERATED_BODY()

public:
	/** Handle an incoming bunch. */
	virtual void ReceivedBunch(FBunch& Bunch) override;

	FSafeDelegate<FEchoMessage&> ReceivedMessageDelegate;
};