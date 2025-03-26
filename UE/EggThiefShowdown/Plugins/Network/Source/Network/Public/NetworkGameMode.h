// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "NetworkGameMode.generated.h"


class UHaNetDriver;
UCLASS(config = Network)
class NETWORK_API ANetworkGameMode : public AGameModeBase, public FNetworkNotify
{
	GENERATED_BODY()
	
public:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Transient)
	UHaNetDriver* NetDriver = nullptr;

	UPROPERTY(Config)
	FString Host;

	UPROPERTY(Config)
	int32 Port;
};
