#pragma once
#include "EngineMinimal.h"
#include "NetworkDelegates.h"
#include "ServerGameMode.generated.h"

class UIpNetDriver;
class UDBNetDriver;
class UDBChannel;
class UIpConnection;

UCLASS()
class GAMEPROJECT_API AServerGameMode : public AGameModeBase, public FNetworkNotify
{
	GENERATED_BODY()
public:
	AServerGameMode();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSceonds) override;

	virtual void NotifyAcceptedConnection(class UNetConnection* Connection) override;
	virtual void NotifyConnectionClosed(class UNetConnection* Connection) override;

public: // LoginNetDriver
	void OnLogin(const struct FAccount& NewAccount, UIpConnection* Connection);
	void OnReceivedLogin(UIpConnection* Connection, class FHaLogin& LoginPacket);
	void OnChatMessage(UIpConnection*, class FChatMessage& NewMessage);

public: // ChannelNetDriver
	void OnRequestCheckAccount(UIpConnection* Connection, class FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValid& Bunch);

protected: // DB
	TObjectPtr<UDBNetDriver> DBNetDriver;
	UDBChannel* DBChannel = nullptr;

protected: // UE 클라 및 자체 클라
	TObjectPtr<UIpNetDriver> LoginNetDriver;

protected:
	FString GetUsername(UIpConnection* Connection);
	map<string, UIpConnection*> LoginUsers;

protected: // UE 데디 서버
	TObjectPtr<UIpNetDriver> UEServerChannelNetDriver;
	TArray<UIpConnection*> UEDediServers;
};