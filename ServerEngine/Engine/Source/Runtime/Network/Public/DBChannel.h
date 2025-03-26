#pragma once

#include "Engine/Channel.h"
#include <boost/describe.hpp>
#include "DBChannel.generated.h"

struct FAccount
{
	string UserName;
	string Password;
};
BOOST_DESCRIBE_STRUCT(FAccount, (), (UserName, Password))

enum ELoginResult : uint32
{
	None,
	Success,
	UsernameNotFound,
	PasswordError,
};

UCLASS()
class NETWORK_API UDBChannel : public UChannel
{
	GENERATED_BODY()

public:
	virtual void Init(UNetConnection* InConnection, int32 InChIndex) override;

	void CreateAccount(const FAccount& InAccount, function<void(bool)> ResultFunction);
	void Login(const FAccount& InAccount, function<void(ELoginResult)> ResultFunction);

protected:
	class UDBNetDriver* DBNetDriver = nullptr;
};