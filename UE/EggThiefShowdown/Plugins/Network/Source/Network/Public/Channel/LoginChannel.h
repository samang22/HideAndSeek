#pragma once

#include "CoreMinimal.h"
#include "Channel/SendRecvBunchChannel.h"
#include "LoginChannel.generated.h"

template<uint8 MessageType> class FNetLoginMessage {};

#define DEFINE_LOGIN_CHANNEL_MESSAGE(Name, Index) DEFINE_CHANNEL_MESSAGE(Login, 3, Name, Index)

DEFINE_LOGIN_CHANNEL_MESSAGE(CTS_CreateAccount, 0);
DEFINE_LOGIN_CHANNEL_MESSAGE(STC_CreateAccount, 1);

#pragma pack(push, 1)
class FHaLogin : public FBunch
{
public:
    bool bCreateAccount = false;
    WIDECHAR UserName[15] = {};
    WIDECHAR Password[15] = {};
};
#pragma pack(pop)

UCLASS()
class NETWORK_API ULoginChannel : public USendRecvBunchChannel
{
    GENERATED_BODY()

public:
    /** Handle an incoming bunch. */
    virtual void ReceivedBunch(FBunch& Bunch) override;
};