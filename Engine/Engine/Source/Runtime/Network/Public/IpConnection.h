#pragma once
#include "CoreMinimal.h"
#include "Engine/NetConnection.h"
#include "NetworkFwd.h"
#include <boost/pool/pool.hpp>
#include "IpConnection.generated.h"

UCLASS()
class NETWORK_API UIpConnection : public UNetConnection
{
	GENERATED_BODY()

public:
	virtual bool InitRemoteConnection(UNetDriver* InDriver, const FURL& InURL, FIOContext& InContext);
	// 클라가 서버에 접속하면 서버에서 호출
	virtual void OnAccepted() override;
	// 서버에 connect 성공 하면 클라에서 호출
	virtual void OnConnected() override;
	virtual void HandleConnectionEstablished() override;

	virtual void ReceivedRawPacket(uint64 InRecvSize) override; // 수신한 데이터는 RecvBuffer에 있습니다.
	virtual void LowLevelSend(void* Data, int32 CountBits) override;

	virtual void Shutdown();

public:
	FSocket* GetSocket() const { return Socket.get(); }
	const FEndPoint& GetRemoteEndPoint() const { return RemoteEndPoint; }

	void SetRecvBuffer(const uint32 InRecvBufferSize) { RecvBufferSize = InRecvBufferSize; }
	TArray<uint8>& GetRecvBuffer() { return RecvBuffer; }
	uint32 GetRecvBufferSize() const { return RecvBufferSize; }

protected:
	virtual bool InitBase(UNetDriver* InDriver, const FURL& InURL, FIOContext& InContext);

protected:
	unique_ptr<FSocket> Socket;
	FEndPoint RemoteEndPoint;
	uint32 RecvBufferSize = 4096;
	TArray<uint8> RecvBuffer;

	std::unique_ptr<boost::pool<>> BufferPool;
};