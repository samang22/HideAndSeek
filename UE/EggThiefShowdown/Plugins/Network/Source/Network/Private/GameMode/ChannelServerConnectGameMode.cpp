// Fill out your copyright notice in the Description page of Project Settings.


#include "GameMode/ChannelServerConnectGameMode.h"
#include "Subsystem/HaServerSubsystem.h"
#include "Channel/UEDediServerChannel.h"
#include "GameFramework/GameSession.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/PlayerState.h"

AChannelServerConnectGameMode::AChannelServerConnectGameMode()
{
}

void AChannelServerConnectGameMode::PreLoginAsync(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, const FOnPreLoginCompleteDelegate& OnComplete)
{
	int32 NumPlayers = GetNumPlayers() + PendingLogins.Num();
	if (GameSession->MaxPlayers <= NumPlayers)
	{
		Super::PreLoginAsync(Options, Address, UniqueId, OnComplete);
		return;
	}

	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));
	const FString Password = UGameplayStatics::ParseOption(Options, TEXT("Password"));
	if (UserName.IsEmpty() || Password.IsEmpty())
	{
		Super::PreLoginAsync(Options, Address, UniqueId, OnComplete);
		return;
	}
	else if (UserName.Len() > 15 || Password.Len() > 15)
	{
		Super::PreLoginAsync(Options, Address, UniqueId, OnComplete);
		return;
	}

	for (int32 i = 0; i < PendingLogins.Num(); ++i)
	{
		if (PendingLogins[i].UserName == UserName)
		{
			Super::PreLoginAsync(Options, Address, UniqueId, OnComplete);
			return;
		}
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->PlayerState && !MustSpectate(PC))
		{
			const FString PlayerName = PC->GetPlayerState<APlayerState>()->GetPlayerName();
			if (PlayerName == UserName)
			{
				Super::PreLoginAsync(Options, Address, UniqueId, OnComplete);
				GameSession->KickPlayer(PC, FText::FromString(TEXT("중복 로그인")));
				break;
			}
		}
	}

	PendingLogins.Emplace(Options, Address, UniqueId, OnComplete, UserName, Password);
	NumPlayers = GetNumPlayers() + PendingLogins.Num();

	// 서버로 UserName과 Password가 유효한지 검증
	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();

	FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValid Ha_DEDI_TO_LOGIN_SERVER_CheckAccountValid;
	std::wcsncpy(Ha_DEDI_TO_LOGIN_SERVER_CheckAccountValid.UserName, &UserName[0], UserName.Len());
	std::wcsncpy(Ha_DEDI_TO_LOGIN_SERVER_CheckAccountValid.Password, &Password[0], Password.Len());
	FNetDediServerChannelMessage<NMT_UETS_CheckAccountValid>::Send(Subsystem->GetServerConnection(), Ha_DEDI_TO_LOGIN_SERVER_CheckAccountValid);

	FHaDediServerChannelInfo HaDediServerChannelInfo;
	HaDediServerChannelInfo.MaxPlayers = GameSession->MaxPlayers;
	HaDediServerChannelInfo.CurrentPlayers = NumPlayers;
	HaDediServerChannelInfo.Port = GetWorld()->URL.Port;
	FNetDediServerChannelMessage<NMT_UETS_Info>::Send(Subsystem->GetServerConnection(), HaDediServerChannelInfo);
}

void AChannelServerConnectGameMode::PreLogin(const FString& Options, const FString& Address, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	Super::PreLogin(Options, Address, UniqueId, ErrorMessage);

	const int32 NumPlayers = GetNumPlayers() + PendingLogins.Num();
	if (GameSession->MaxPlayers <= NumPlayers)
	{
		ErrorMessage = TEXT("풀방입니다");
		return;
	}

	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));
	const FString Password = UGameplayStatics::ParseOption(Options, TEXT("Password"));
	if (UserName.IsEmpty() || Password.IsEmpty())
	{
		ErrorMessage = TEXT("UserName 또는 Password가 없습니다");
		return;
	}
	else if (UserName.Len() > 15 || Password.Len() > 15)
	{
		ErrorMessage = TEXT("UserName 또는 Password가 정책을 위반 했습니다");
		return;
	}

	for (int32 i = 0; i < PendingLogins.Num(); ++i)
	{
		if (PendingLogins[i].UserName == UserName)
		{
			ErrorMessage = TEXT("동일한 계정이 접속 대기 중입니다. 잠시후 다시 시도해 주세요");
			return;
		}
	}

	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->PlayerState && !MustSpectate(PC))
		{
			const FString PlayerName = PC->GetPlayerState<APlayerState>()->GetPlayerName();
			if (PlayerName == UserName)
			{
				ErrorMessage = TEXT("동일한 계정이 접속 중입니다. 로그 아웃 처리 중이오니 잠시후 다시 시도해 주세요");
				return;
			}
		}
	}
}

APlayerController* AChannelServerConnectGameMode::Login(UPlayer* NewPlayer, ENetRole InRemoteRole, const FString& Portal, const FString& Options, const FUniqueNetIdRepl& UniqueId, FString& ErrorMessage)
{
	APlayerController* PC = Super::Login(NewPlayer, InRemoteRole, Portal, Options, UniqueId, ErrorMessage);

	const FString UserName = UGameplayStatics::ParseOption(Options, TEXT("UserName"));
	ChangeName(PC, UserName, true);

	GetWorld()->GetTimerManager().SetTimerForNextTick([this]()
		{
			FString TravelURL = TEXT("/Game/Level/LobbyMap?game=/Game/GameMode/BPGM_LobbyMap.BPGM_LobbyMap_C");
			GetWorld()->ServerTravel(TravelURL);
		});

	return PC;
}

void AChannelServerConnectGameMode::Logout(AController* Exiting)
{
	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	int32 NumPlayers = GetNumPlayers() + PendingLogins.Num() - 1;
	FHaDediServerChannelInfo HaDediServerChannelInfo;
	HaDediServerChannelInfo.MaxPlayers = GameSession->MaxPlayers;
	HaDediServerChannelInfo.CurrentPlayers = NumPlayers;
	HaDediServerChannelInfo.Port = GetWorld()->URL.Port;
	FNetDediServerChannelMessage<NMT_UETS_Info>::Send(Subsystem->GetServerConnection(), HaDediServerChannelInfo);

	Super::Logout(Exiting);
}

void AChannelServerConnectGameMode::BeginPlay()
{
	Super::BeginPlay();

	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();
	Subsystem->ConnectToServer(Host, Port);
	DediServerChannel = Subsystem->GetDediServerChannel();
	if (!DediServerChannel) { return; }
	DediServerChannel->OnCheckAccountValidResult.AddDynamic(this, &ThisClass::OnCheckAccountValidResultFunction);
	DediServerChannel->OnKickPlayer.AddDynamic(this, &ThisClass::OnKickPlayerFunction);

	FHaDediServerChannelInfo HaDediServerChannelInfo;
	HaDediServerChannelInfo.MaxPlayers = GameSession->MaxPlayers;
	HaDediServerChannelInfo.CurrentPlayers = 0;
	HaDediServerChannelInfo.Port = GetWorld()->URL.Port;
	FNetDediServerChannelMessage<NMT_UETS_Info>::Send(Subsystem->GetServerConnection(), HaDediServerChannelInfo);
}

void AChannelServerConnectGameMode::OnCheckAccountValidResultFunction(bool bResult, FString UserName, FString Password)
{
	UHaServerSubsystem* Subsystem = GetGameInstance()->GetSubsystem<UHaServerSubsystem>();

	// 여기서 계정 검증 결과를 확인하고 접속자를 받거나 받지 않는다
	for (int32 i = 0; i < PendingLogins.Num(); ++i)
	{
		if (PendingLogins[i].UserName == UserName)
		{
			FPendingLogin PendingLogin = PendingLogins[i];
			PendingLogins.RemoveAt(i);

			FString ErrorMessage;
			if (!bResult)
			{
				const int32 NumPlayers = GetNumPlayers() + PendingLogins.Num();
				ErrorMessage = TEXT("ID 및 Password 인증 실패");
				FHaDediServerChannelInfo HaDediServerChannelInfo;
				HaDediServerChannelInfo.MaxPlayers = GameSession->MaxPlayers;
				HaDediServerChannelInfo.CurrentPlayers = NumPlayers;
				HaDediServerChannelInfo.Port = GetWorld()->URL.Port;
				FNetDediServerChannelMessage<NMT_UETS_Info>::Send(Subsystem->GetServerConnection(), HaDediServerChannelInfo);
			}
			PreLogin(PendingLogin.Options, PendingLogin.Address, PendingLogin.UniqueId, ErrorMessage);
			PendingLogin.OnComplete.ExecuteIfBound(ErrorMessage);

			return;
		}
	}
}

void AChannelServerConnectGameMode::OnKickPlayerFunction(FString UserName)
{
	for (FConstPlayerControllerIterator Iterator = GetWorld()->GetPlayerControllerIterator(); Iterator; ++Iterator)
	{
		APlayerController* PC = Iterator->Get();
		if (PC && PC->PlayerState && !MustSpectate(PC))
		{
			const FString PlayerName = PC->GetPlayerState<APlayerState>()->GetPlayerName();
			if (PlayerName == UserName)
			{
				GameSession->KickPlayer(PC, FText::FromString(TEXT("중복 로그인")));
				break;
			}
		}
	}
}
