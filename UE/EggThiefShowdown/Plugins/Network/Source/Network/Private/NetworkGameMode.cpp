// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameMode.h"
#include "HaNetDriver.h"
#include "EchoChannel.h"

ANetworkGameMode::ANetworkGameMode()
{
	PrimaryActorTick.bCanEverTick = true;
}

void ANetworkGameMode::BeginPlay()
{
	Super::BeginPlay();

	NetDriver = NewObject<UHaNetDriver>(this, TEXT("HaNetDriver"));

	FURL URL;
	URL.Host = Host;
	URL.Port = Port;

	FString OutError;
	if (!NetDriver->InitConnect(this, URL, OutError))
	{
		UE_LOG(LogTemp, Error, TEXT("Connect Failed"));
		RequestEngineExit(TEXT("Server Connect Failed"));
		return;
	}


	FString Message = TEXT("HelloUE!");
	UEchoChannel* EchoChannel = Cast<UEchoChannel>(NetDriver->ServerConnection->Channels[1]);
	FEchoMessage EchoMessage;
	std::wcsncpy(EchoMessage.Message, &Message[0], Message.Len());
	FNetEchoMessage<NMT_Echo>::Send(NetDriver->ServerConnection, EchoMessage);
}

void ANetworkGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	NetDriver->TickDispatch(DeltaSeconds);
}
