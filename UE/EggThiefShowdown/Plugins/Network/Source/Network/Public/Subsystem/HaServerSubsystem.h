// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HaServerSubsystem.generated.h"

class UHaNetDriver;
class UEchoChannel;
class UChatChannel;

UCLASS(config = Network)
class NETWORK_API UHaServerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject, public FNetworkNotify
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UEchoChannel* GetEchoChannel() { return EchoChannel; }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UChatChannel* GetChatChannel() { return ChatChannel; }
    UFUNCTION(BlueprintCallable)
    void SendChatMessage(FString InMessage);
public:
    // 기본값으로 인자를 주는 경우 DefaultNetwork.ini설정을 따릅니다
    virtual void ConnectToServer(FStringView InHost = TEXT(""), int32 InPort = -1);


protected:
    // FTickableGameObject 인터페이스 구현
    virtual void Tick(float DeltaTime) override;
    virtual bool IsTickable() const override;
    virtual TStatId GetStatId() const override;;

protected:
    UPROPERTY(Transient)
    UHaNetDriver* NetDriver = nullptr;

    UPROPERTY(Config)
    FString Host;

    UPROPERTY(Config)
    int32 Port;

protected:
    UEchoChannel* EchoChannel = nullptr;
    UChatChannel* ChatChannel = nullptr;
};