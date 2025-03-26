#pragma once

#include "Channel.h"
#include "ControlChannel.generated.h"

/**
 * 연결 제어 메시지를 교환하는 채널입니다.
 */
UCLASS()
class ENGINE_API UControlChannel : public UChannel
{
	GENERATED_BODY()

public:
	/** Handle an incoming bunch. */
	virtual void ReceivedBunch(FBunch& Bunch);
};