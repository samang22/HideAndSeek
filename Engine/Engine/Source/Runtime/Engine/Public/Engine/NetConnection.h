#pragma once
#include "CoreMinimal.h"
#include "Engine/Player.h"
#include "Engine/EngineBaseTypes.h"
#include "NetConnection.generated.h"

class UNetDriver;
class UChannel;

enum EConnectionState : uint8
{
	USOCK_Invalid = 0, // Connection is invalid, possibly uninitialized.
	USOCK_Closed = 1, // Connection permanently closed.
	USOCK_Pending = 2, // Accept는 했으나, 아직 아무런 Packet도 받지 못한
	USOCK_Open = 3, // Connection is open.
};

UCLASS()
class ENGINE_API UNetConnection : public UPlayer
{
	GENERATED_BODY()

public:
	/**
	 * 원격 소스에서 이 연결 인스턴스를 초기화합니다.
	 *
	 * @param InDriver 이 연결과 관련된 네트 드라이버
	 * @param InSocket 이 연결과 관련된 소켓
	 * @param InURL 초기화할 URL
	 * @param InRemoteAddr 이 연결의 원격 주소
	 * @param InState 이 연결에 대해 시작할 연결 상태
	 * @param InMaxPacket 전송에 사용할 최대 패킷 크기
	 * @param InPacketOverhead 이 연결 유형에 대한 패킷 오버헤드
	 */
	virtual bool InitRemoteConnection(UNetDriver* InDriver, const FURL& InURL);
	// 클라가 서버에 접속하면 서버에서 호출
	virtual void OnAccepted() { HandleConnectionEstablished(); }
	// 서버에 connect 성공 하면 클라에서 호출
	virtual void OnConnected() { HandleConnectionEstablished(); }
	// 서버 또는 클라 접속에 성공하면 호출
	virtual void HandleConnectionEstablished();

	/**
	 * 수신된 원시 데이터를 처리할 수 있도록 연결을 허용합니다.
	 *
	 * @param Data 처리할 데이터 (우리는 UIpConnection의 RecvBuffer를 사용)
	 * @param Count 처리할 데이터 버퍼의 크기
	 */
	virtual void ReceivedRawPacket(uint64 InRecvSize) {}
	/**
	 * 바이트 스트림을 소켓을 사용하여 원격 엔드포인트로 전송합니다.
	 *
	 * @param Data         전송할 바이트 스트림
	 * @param CountBits    전송할 스트림의 길이(비트 단위)
	 */
	virtual void LowLevelSend(void* Data, int32 CountBits) {}

	//// 내부에서 PacketSize를 계산합니다.
	//template<class T>
	//void SendPacket(T& InPacket)
	//{
	//	static_assert(std::is_base_of<FPacketHeader, T>::value, "T must inherit from FPacketHeader");
	//	FPacketHeader* PacketHeader = static_cast<FPacketHeader*>(&InPacket);
	//	PacketHeader->PacketSize = sizeof(T);
	//	LowLevelSend((void*)&InPacket, sizeof(T));
	//}

	void SetConnectionState(EConnectionState NewState) { ConnectionState = NewState; }

	const FString& GetRemoteIP() const { return RemoteIP; }
	const uint16 GetRemotePort() const { return RemotePort; }

	bool IsServer() const;

	UNetDriver* GetNetDriver() { return NetDriver; }

	virtual void Shutdown() {}

public:
	TArray<TObjectPtr<UChannel>> Channels;

protected:
	virtual bool InitBase(UNetDriver* InDriver, const FURL& InURL);

protected:
	UNetDriver* NetDriver = nullptr;
	FURL URL;
	EConnectionState ConnectionState = EConnectionState::USOCK_Invalid;

	FString RemoteIP;
	uint16 RemotePort = 0;
};