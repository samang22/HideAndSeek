#pragma once
#include "CoreMinimal.h"
#include "IpConnection.h"
#include "ChannelConnection.generated.h"

UCLASS()
class NETWORK_API UChannelConnection : public UIpConnection
{
	GENERATED_BODY()

public:
	virtual void HandleConnectionEstablished() override;
};