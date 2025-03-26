// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IpConnection.h"
#include "HaConnection.generated.h"

/**
 * 
 */
UCLASS()
class NETWORK_API UHaConnection : public UIpConnection
{
	GENERATED_BODY()

	friend class UHaNetDriver;

public:
	UHaConnection();
	// To use TCP not UDP, we override this class
	virtual void LowLevelSend(void* Data, int32 CountBits, FOutPacketTraits& Traits) override;

	virtual void OnConnect();

	virtual class FBunch* GetPacket();

protected:
	bool ReadPacketSome(const uint32 InReadSize);
	bool ReadPacket();

protected:
	TArray<uint8> RecvBuffer;
	int32 RecvPacketSize = 0;
};
