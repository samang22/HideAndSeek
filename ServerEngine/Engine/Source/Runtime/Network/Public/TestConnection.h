#pragma once
#include "CoreMinimal.h"
#include "IpConnection.h"
#include "TestConnection.generated.h"

UCLASS()
class NETWORK_API UTestConnection : public UIpConnection
{
	GENERATED_BODY()

public:
	virtual void HandleConnectionEstablished() override;
};