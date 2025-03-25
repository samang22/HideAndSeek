// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "IpNetDriver.h"
#include "HaNetDriver.generated.h"

class UHaConneciton;
UCLASS()
class NETWORK_API UHaNetDriver : public UIpNetDriver
{
	GENERATED_BODY()
	
public:
	virtual bool InitConnectionClass(void) override;
	virtual bool InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;

// Override to use TCP, not UDP
protected:
	/**
	 * Creates a socket set up for communication using the given protocol. This allows for explicit creation instead of inferring type for you.
	 *
	 * @param ProtocolType	an FName that represents the protocol to allocate the new socket under. Typically set to None or a value in FNetworkProtocolTypes
	 * @return				an FSocket if creation succeeded, nullptr if creation failed.
	 */
	virtual FUniqueSocket CreateSocketForProtocol(const FName& ProtocolType) override;

	/**
	 * Creates, initializes and binds a socket using the given bind address information.
	 *
	 * @param BindAddr				the address to bind the new socket to, will also create the socket using the address protocol using CreateSocketForProtocol
	 * @param Port					the port number to use with the given bind address.
	 * @param bReuseAddressAndPort	if true, will set the socket to be bound even if the address is in use
	 * @param DesiredRecvSize		the max size of the recv buffer for the socket
	 * @param DesiredSendSize		the max size of the sending buffer for the socket
	 * @param Error					a string reference that will be populated with any error messages should an error occur
	 *
	 * @return if the socket could be created and bound with all the appropriate options, a pointer to the new socket is given, otherwise null
	 */
	virtual FUniqueSocket CreateAndBindSocket(TSharedRef<FInternetAddr> BindAddr, int32 Port, bool bReuseAddressAndPort, int32 DesiredRecvSize, int32 DesiredSendSize, FString& Error) override;
};
