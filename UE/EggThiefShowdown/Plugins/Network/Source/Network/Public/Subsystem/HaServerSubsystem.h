// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "HaServerSubsystem.generated.h"

class UHaNetDriver;
class UEchoChannel;
class UChatChannel;
class ULoginChannel;
class UUEDediServerChannel;

UCLASS(config = Network)
class NETWORK_API UHaServerSubsystem : public UGameInstanceSubsystem, public FTickableGameObject, public FNetworkNotify
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UEchoChannel* GetEchoChannel() { return EchoChannel; }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    UChatChannel* GetChatChannel() { return ChatChannel; }
    UFUNCTION(BlueprintCallable, BlueprintPure)
    ULoginChannel* GetLoginChannel() { return LoginChannel; }
    void RequestDediServerInfo();

public:
    // LoginChannel
    FString GetLoginUserName() const;
    FString GetLoginPassword() const;

public: // Dedi Server only
    UUEDediServerChannel* GetDediServerChannel() { return DediServerChannel; }

public:
    UFUNCTION(BlueprintCallable)
    void SendChatMessage(FString InMessage);

    void SendCreateAccount(FString InUsername, FString InPassword);
    void SendLogin(FString InUsername, FString InPassword);

    EConnectionState GetConnectionState();
    UNetConnection* GetServerConnection();


public:
    // 기본값으로 인자를 주는 경우 DefaultNetwork.ini설정을 따릅니다
    virtual void ConnectToServer(FStringView InHost = TEXT(""), int32 InPort = -1);

protected:
    void SendLoginOrCreateAccount(bool bCreateAccount, const FString& InUsername, const FString& InPassword);

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
    // UE Client가 자체 서버와 통신하는 Channel
    UEchoChannel* EchoChannel = nullptr;
    UChatChannel* ChatChannel = nullptr;
    ULoginChannel* LoginChannel = nullptr;
protected:
    // UE Deci Server가 자체 서버와 통신하는 Channel
    // 편의상 분리하지 않고 나머지 Channel을 그대로 둔다
    UUEDediServerChannel* DediServerChannel = nullptr;


protected:
    uint8 eLobbyCharacterKind = 0;
public:
    void SetLobbyCharacterKind(uint8 eKind) { eLobbyCharacterKind = eKind; }
    uint8 GetLobbyCharacterKind() const { return eLobbyCharacterKind; }
};