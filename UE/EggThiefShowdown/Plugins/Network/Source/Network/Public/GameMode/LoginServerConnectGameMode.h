// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "LoginServerConnectGameMode.generated.h"

// 클라가 로그인(및 채팅) 서버로 접속하는 게임 모드
UCLASS(config = Network)
class NETWORK_API ALoginServerConnectGameMode : public AGameModeBase, public FNetworkNotify
{
	GENERATED_BODY()
	
protected:
	virtual void BeginPlay() override;

protected:
	UPROPERTY(Config)
	FString Host;

	UPROPERTY(Config)
	int32 Port;
};
