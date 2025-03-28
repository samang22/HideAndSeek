#pragma once

#include "CoreMinimal.h"
#include "Channel/SendRecvBunchChannel.h"
#include "LoginChannel.generated.h"

template<uint8 MessageType> class FNetLoginMessage {};

#define DEFINE_LOGIN_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Login, 3, Name, Index)

#pragma pack(push, 1)
DEFINE_LOGIN_CHANNEL_MESSAGE(CTS_CreateAccount, 0);
class FHaLogin : public FBunch
{
public:
    bool bCreateAccount = false;
    WIDECHAR UserName[15] = {};
    WIDECHAR Password[15] = {};
};

DEFINE_LOGIN_CHANNEL_MESSAGE(STC_CreateAccountResult, 1);
class FHaCreateAccountResult : public FBunch
{
public:
    // 0: 실패, 1: 성공
    uint32 ResultCode;
};

DEFINE_LOGIN_CHANNEL_MESSAGE(STC_LoginResult, 2);
class FHaLoginResult : public FBunch
{
public:
	// see ELoginResult
	/*enum ELoginResult : uint32
	{
		None,
		Success,
		UsernameNotFound,
		PasswordError,
	};*/
	uint32 ResultCode;
	WIDECHAR UserName[15] = {};
	WIDECHAR Password[15] = {};
};
// 클라가 서버로 데디서버 정보를 요청하는 Packet
DEFINE_LOGIN_CHANNEL_MESSAGE(CTS_DediServerInfo, 3);
class FHaRequestDediServerInfo : public FBunch
{
};

// 서버가 클라로 데디서버 정보를 전달하는 Packet
DEFINE_LOGIN_CHANNEL_MESSAGE(STC_DediServerInfo, 4);
class FHaDediServerInfo : public FBunch
{
public:
	// 최대 서버 수가 20으로 가정
	const static uint32 MaxServerCount = 20;
	uint16 ServerCount;
	char ServerIP[MaxServerCount][15];
	uint16 MaxPlayers[MaxServerCount] = {};
	uint16 CurrentPlayers[MaxServerCount] = {};
	uint16 Ports[MaxServerCount] = {};
};
#pragma pack(pop)

struct FDediServerInfo
{
	FString IP;
	int32 MaxPlayers = -1;
	int32 CurrentPlayers = -1;
	int32 Port = -1;
};

// 0: 실패, 1: 성공
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCreateAccountResult, uint32, ResultCode);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnLoginResult, uint32, ResultCode);

UCLASS()
class NETWORK_API ULoginChannel : public USendRecvBunchChannel
{
    GENERATED_BODY()

public:
    /** Handle an incoming bunch. */
    virtual void ReceivedBunch(FBunch& Bunch) override;

public:
	FOnCreateAccountResult OnCreateAccountResult;
	FOnLoginResult OnLoginResult;

	int32 GetServerCount() const { return ServerCount; }
	//const TArray<FDediServerInfo>& GetDediServerInfos() const { return DediServerInfos; }

protected:
	FString LoginUserName;
	FString LoginPassword;

protected:
	int32 ServerCount = -1;
	//TArray<FDediServerInfo> DediServerInfos;
};