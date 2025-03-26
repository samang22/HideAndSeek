#pragma once
#include "Engine/NetDriver.h"
#include "NetworkFwd.h"
#include "IpNetDriver.generated.h"

class UIpConnection;

UCLASS()
class NETWORK_API UIpNetDriver : public UNetDriver
{
	GENERATED_BODY()

public:
	UIpNetDriver();
	virtual bool InitListen(class FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error) override;
	virtual bool InitConnect(class FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;

	virtual void Tick(float DeltaSeconds) override;

	virtual UIpConnection* GetServerConnection();

	map<UIpConnection*, TObjectPtr<UIpConnection>>& GetConnections() { return Connections; }

	// 해당 Connection을 수동으로 끊습니다
	virtual void CloseConnection(UNetConnection* Connection) override;

protected:
	virtual bool InitBase(bool bInitAsClient, class FNetworkNotify* InNotify, const FURL& URL, bool bReuseAddressAndPort, FString& Error) override;

	virtual void StartAceept();
	virtual void AsyncRead(UIpConnection* IpConnection);

protected:
	// - socket 생성
	// - bind(포트와 IP를 socket에 연결)
	// - Listen
	// - Accept
	// 처리를 하는 boost asio class이다
	shared_ptr<FAcceptor> Acceptor;
	FIOContext Context;

	map<UIpConnection*, TObjectPtr<UIpConnection>> Backlogs;
	map<UIpConnection*, TObjectPtr<UIpConnection>> Connections;

protected:
	TObjectPtr<UIpConnection> ServerConnection;
};