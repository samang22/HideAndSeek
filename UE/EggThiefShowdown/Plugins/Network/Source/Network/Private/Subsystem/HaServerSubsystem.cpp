// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystem/HaServerSubsystem.h"
#include "HaNetDriver.h"
#include "Channel/EchoChannel.h"
#include "Channel/ChatChannel.h"
#include "Channel/LoginChannel.h"
#include "Channel/UEDediServerChannel.h"


void UHaServerSubsystem::RequestDediServerInfo()
{
    FHaRequestDediServerInfo HaRequestDediServerInfo;
    FNetLoginMessage<NMT_CTS_DediServerInfo>::Send(NetDriver->ServerConnection, HaRequestDediServerInfo);

}

FString UHaServerSubsystem::GetLoginUserName() const
{
    return LoginChannel->GetLoginUserName();
}

FString UHaServerSubsystem::GetLoginPassword() const
{
    return LoginChannel->GetLoginPassword();
}

void UHaServerSubsystem::SendChatMessage(FString InMessage)
{
    if (InMessage.IsEmpty() || InMessage.Len() > UE_ARRAY_COUNT(FHaChatMessage::Message))
    {
        return;
    }

    FHaChatMessage HaChatMessage;
    std::wcsncpy(HaChatMessage.Message, &InMessage[0], InMessage.Len());
    FNetChatMessage<NMT_CTS_Chat>::Send(NetDriver->ServerConnection, HaChatMessage);
}

void UHaServerSubsystem::SendCreateAccount(FString InUsername, FString InPassword)
{
    SendLoginOrCreateAccount(true, InUsername, InPassword);
}

void UHaServerSubsystem::SendLogin(FString InUsername, FString InPassword)
{
    SendLoginOrCreateAccount(false, InUsername, InPassword);
}

EConnectionState UHaServerSubsystem::GetConnectionState()
{
    if (!NetDriver->ServerConnection)
    {
        return EConnectionState::USOCK_Closed;
    }

    return NetDriver->ServerConnection->GetConnectionState();
}

UNetConnection* UHaServerSubsystem::GetServerConnection()
{
    return NetDriver->ServerConnection;
}


void UHaServerSubsystem::ConnectToServer(FStringView InHost, int32 InPort)
{
    NetDriver = NewObject<UHaNetDriver>(this, TEXT("Ha NetDriver"));
    FURL URL;
    URL.Host = InHost.IsEmpty() ? Host : InHost;
    URL.Port = InPort == -1 ? Port : InPort;

    check(!URL.Host.IsEmpty());
    check(URL.Port != -1);

    FString OutError;

    if (!NetDriver->InitConnect(this, URL, OutError))
    {
        check(false);
        UE_LOG(LogTemp, Error, TEXT("Connect Failed"));
        //RequestEngineExit(TEXT("Connect Failed"));
        return;
    }

    // UE Client only
#if !UE_SERVER
    EchoChannel = Cast<UEchoChannel>(NetDriver->ServerConnection->Channels[1]);
    ChatChannel = Cast<UChatChannel>(NetDriver->ServerConnection->Channels[2]);
    LoginChannel = Cast<ULoginChannel>(NetDriver->ServerConnection->Channels[3]);
#else
    // UE Server only
    DediServerChannel = Cast<UUEDediServerChannel>(NetDriver->ServerConnection->Channels[4]);
#endif
}

void UHaServerSubsystem::SendLoginOrCreateAccount(bool bCreateAccount, const FString& InUsername, const FString& InPassword)
{
    if (InUsername.IsEmpty() || InPassword.IsEmpty())
    {
        check(false);
        return;
    }

    FString UserName = InUsername;
    FString Password = InPassword;
    FHaLogin Login;
    Login.bCreateAccount = bCreateAccount;
    std::wcsncpy(Login.UserName, &UserName[0], UserName.Len());
    std::wcsncpy(Login.Password, &Password[0], Password.Len());
    FNetLoginMessage<NMT_CTS_CreateAccount>::Send(NetDriver->ServerConnection, Login);
}

void UHaServerSubsystem::Tick(float DeltaSeconds)
{
    if (!NetDriver)
    {
        return;
    }

    NetDriver->TickDispatch(DeltaSeconds);
}

bool UHaServerSubsystem::IsTickable() const
{
    return true; // 항상 틱 가능하도록 설정
}

TStatId UHaServerSubsystem::GetStatId() const
{
    // 통계 ID를 반환합니다.
    RETURN_QUICK_DECLARE_CYCLE_STAT(UHaServerSubsystem, STATGROUP_Tickables);
}