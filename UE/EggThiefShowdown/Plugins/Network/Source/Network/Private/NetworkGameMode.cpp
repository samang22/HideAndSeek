// Fill out your copyright notice in the Description page of Project Settings.


#include "NetworkGameMode.h"
#include "HaNetDriver.h"

void ANetworkGameMode::BeginPlay()
{
	Super::BeginPlay();

	NetDriver = NewObject<UHaNetDriver>(this, TEXT("HaNetDriver"));

	FURL URL;
	URL.Host = TEXT("127.0.0.1");
	URL.Port = 5555;
	FString OutError;
	if (!NetDriver->InitConnect(this, URL, OutError))
	{
		UE_LOG(LogTemp, Error, TEXT("Connect Failed"));
		RequestEngineExit(TEXT("Server Connect Failed"));
		return;
	}

	{
		int a = 0;
	}
}
