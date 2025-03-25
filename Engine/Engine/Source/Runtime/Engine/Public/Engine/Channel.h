#pragma once

#include "CoreMinimal.h"
#include "Channel.generated.h"

class UNetConnection;
class UNetDriver;
class FBunch;

/**
 * 통신 채널의 기본 클래스.
 */
UCLASS(/*abstract, transient, MinimalAPI*/)
class ENGINE_API UChannel : public UObject
{
	GENERATED_BODY()

public:
	UNetConnection* Connection = nullptr;		// Owner connection.
	UNetDriver* NetDriver = nullptr;			// Owner NetDriver.

	int32				ChIndex;			// Index of this channel.

	/**
	 * UChannel 인터페이스.
	 */
	 /** 주어진 연결과 인덱스에 대해 이 채널을 초기화합니다. */
	virtual void Init(UNetConnection* InConnection, int32 InChIndex/*, EChannelCreateFlags CreateFlags*/);
	
	/**
	 * 오버플로되지 않은 경우 bunch를 보내고, 신뢰성이 있는 경우 이를 대기열에 추가합니다.
	 */
	virtual void SendBunch(FBunch& Bunch/*, bool Merge*/);
	
	/** Handle an incoming bunch. */
	virtual void ReceivedBunch(FBunch& Bunch) {}
};