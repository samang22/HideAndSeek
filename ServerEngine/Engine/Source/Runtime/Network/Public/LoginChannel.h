#pragma once

#include "Engine/Channel.h"
#include <boost/describe.hpp>
#include "Net/DataBunch.h"
#include "LoginChannel.generated.h"

template<uint8 MessageType> class FNetLoginMessage {};

#define DEFINE_LOGIN_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Login, 3, Name, Index)

#pragma pack(push, 1)
DEFINE_LOGIN_CHANNEL_MESSAGE(CTS_CreateAccount, 0);
class FARLogin : public FBunch
{
public:
	bool bCreateAccount = false;
	WIDECHAR UserName[15] = {};
	WIDECHAR Password[15] = {};
};

DEFINE_LOGIN_CHANNEL_MESSAGE(STC_CreateAccountResult, 1);
class FARCreateAccountResult : public FBunch
{
public:
	// 0: 실패, 1: 성공
	uint32 ResultCode;
};

DEFINE_LOGIN_CHANNEL_MESSAGE(STC_LoginResult, 2);
class FARLoginResult : public FBunch
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
class FARRequestDediServerInfo : public FBunch
{ };

// 서버가 클라로 데디서버 정보를 전달하는 Packet
DEFINE_LOGIN_CHANNEL_MESSAGE(STC_DediServerInfo, 4);
class FARDediServerInfo : public FBunch
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

UCLASS()
class NETWORK_API ULoginChannel : public UChannel
{
	GENERATED_BODY()

public:
	/** Handle an incoming bunch. */
	virtual void ReceivedBunch(FBunch& Bunch) override;

	FSafeDelegate<class UIpConnection*, FARLogin&> ReceivedDelegate;

	FSafeDelegate<class UIpConnection*> RequestDediServerInfo;
};