#include "ServerGameMode.h"
#include "IpNetDriver.h"
#include "TestConnection.h"

#include "DBNetDriver.h"
#include "DBChannel.h"
#include "ChannelConnection.h"
#include "LoginChannel.h"
#include "ChatChannel.h"
#include "UEDediServerChannel.h"
#include "shellapi.h"


AServerGameMode::AServerGameMode()
{
}


void AServerGameMode::BeginPlay()
{
    Super::BeginPlay();

    FString Error;

	// 계정 DB 접속
	FConfigFile& ConfigFile = GConfig->GetConfig(GEngineIni);
	{
		DBNetDriver = NewObject<UDBNetDriver>(this, UDBNetDriver::StaticClass(), TEXT("DBNetDriver0"));

		FURL URL;
		FString ID, Password;
		ConfigFile.Get("DB Server", "Host", URL.Host);
		ConfigFile.Get("DB Server", "Port", URL.Port);
		ConfigFile.Get("DB Server", "ID", ID);
		ConfigFile.Get("DB Server", "Password", Password);
		URL.Op.emplace_back(ID);
		URL.Op.emplace_back(Password);

		if (!DBNetDriver->InitConnect(this, URL, Error))
		{
			E_LOG(Error, TEXT("DBNetDriver connect failed: {}"), Error);
			RequestEngineExit(TEXT("DBNetDriver connect failed"));
			return;
		}

		DBNetDriver->Channels[0] = NewObject<UChannel>(this, UDBChannel::StaticClass(), TEXT("DB"));
		DBNetDriver->Channels[0]->NetDriver = DBNetDriver.get();
		DBNetDriver->Channels[0]->Init(nullptr, 0);
		DBChannel = dynamic_cast<UDBChannel*>(DBNetDriver->Channels[0].get());
	}

	// 로그인 서버
	{
		LoginNetDriver = NewObject<UIpNetDriver>(this, UIpNetDriver::StaticClass(), TEXT("LoginNetDriver"));
		LoginNetDriver->SetNetConnectionClass(UTestConnection::StaticClass());
		FURL URL;
		ConfigFile.Get("Login Server", "Port", URL.Port);

		if (!LoginNetDriver->InitListen(this, URL, false, Error))
		{
			E_LOG(Error, TEXT("Login Server listen failed: {}"), Error);
			RequestEngineExit(TEXT("Login Server listen failed"));
			return;
		}
	}

	//return;

	// 채널 서버
	{
		UEServerChannelNetDriver = NewObject<UIpNetDriver>(this, UIpNetDriver::StaticClass(), TEXT("UE Dedi Server ChannelNetDriver"));
		UEServerChannelNetDriver->SetNetConnectionClass(UChannelConnection::StaticClass());
		FURL URL;
		ConfigFile.Get("Channel Server", "Port", URL.Port);

		if (!UEServerChannelNetDriver->InitListen(this, URL, false, Error))
		{
			E_LOG(Error, TEXT("Channel Server listen failed: {}"), Error);
			RequestEngineExit(TEXT("Channel Server listen failed"));
			return;
		}
		int32 UEServerCount = 0;
		ConfigFile.Get("Channel Server", "UEServerCount", UEServerCount);
		int32 UEServerStartPort = 0;
		ConfigFile.Get("Channel Server", "UEServerStartPort", UEServerStartPort);
		FString UEServerPath;
		ConfigFile.Get("Channel Server", "UEServerPath", UEServerPath);

		for (int32 i = 0; i < UEServerCount; ++i)
		{
			FString Cmd = TEXT("-log -port=") + ANSI_TO_TCHAR(UEServerStartPort + i);
			E_LOG(Log, TEXT("UE Server path: {}, Cmd: {}"), UEServerPath, Cmd);

			SHELLEXECUTEINFO sei = { 0 };
			sei.cbSize = sizeof(SHELLEXECUTEINFO);
			sei.fMask = SEE_MASK_NOCLOSEPROCESS; // 프로세스 핸들 얻기
			sei.lpVerb = L"open";
			sei.lpFile = UEServerPath.c_str(); // 실행할 프로그램
			sei.lpParameters = Cmd.c_str();
			sei.nShow = SW_SHOW;
			if (!ShellExecuteEx(&sei))
			{
				E_LOG(Error, TEXT("UE Server open failed!"));
				RequestEngineExit(TEXT("UE Server open failed!"));
				return;
			}

			//TerminateProcess(sei.hProcess, 0);
			//CloseHandle(sei.hProcess);
		}
	}
}

void AServerGameMode::Tick(float DeltaSceonds)
{
    Super::Tick(DeltaSceonds);

	DBNetDriver->Tick(DeltaSceonds);
	LoginNetDriver->Tick(DeltaSceonds);
	UEServerChannelNetDriver->Tick(DeltaSceonds);
}

void AServerGameMode::NotifyAcceptedConnection(UNetConnection* Connection)
{
	if (Connection->GetNetDriver() == LoginNetDriver.get())
	{
		// 신규 클라 접속
		UChatChannel* ChatChannel = dynamic_cast<UChatChannel*>(Connection->Channels[2].get());
		ChatChannel->OnCTS_Chat.AddUObject(this, &AServerGameMode::OnChatMessage);
		ULoginChannel* LoginChannel = dynamic_cast<ULoginChannel*>(Connection->Channels[3].get());
		LoginChannel->ReceivedDelegate.AddUObject(this, &AServerGameMode::OnReceivedLogin);
		LoginChannel->RequestDediServerInfo.AddUObject(this, &AServerGameMode::OnRequestDediServerInfo);
	}
	else if (Connection->GetNetDriver() == UEServerChannelNetDriver.get())
	{
		// 신규 데디 서버 접속
		UIpConnection* UEDediServer = dynamic_cast<UIpConnection*>(Connection);
		_ASSERT(UEDediServer);
		if (UEDediServer)
		{
			UUEDediServerChannel* DediServerChannel = dynamic_cast<UUEDediServerChannel*>(UEDediServer->Channels[4].get());
			DediServerChannel->RequestCheckAccountDelegate.AddUObject(this, &ThisClass::OnRequestCheckAccount);
			UEDediServers.push_back(UEDediServer);
		}
	}
}

void AServerGameMode::NotifyConnectionClosed(UNetConnection* Connection)
{
	if (Connection->GetNetDriver() == LoginNetDriver.get())
	{
		// 클라가 접속이 끊어짐
		for (auto& It : LoginUsers)
		{
			if (It.second == Connection)
			{
				LoginUsers.erase(It.first);
				break;
			}
		}
	}
	else if (Connection->GetNetDriver() == UEServerChannelNetDriver.get())
	{
		// 데디 서버 연결이 끊어짐 (비정상 상황)
		for (int32 i = 0; i < UEDediServers.size(); ++i)
		{
			if (UEDediServers[i] == Connection)
			{
				UEDediServers.erase(UEDediServers.begin() + i);
				break;
			}
		}
	}
}

void AServerGameMode::OnLogin(const FAccount& NewAccount, UIpConnection* Connection)
{
	LoginUsers.emplace(NewAccount.ID, Connection);
}

void AServerGameMode::OnReceivedLogin(UIpConnection* Connection, FHaLogin& LoginPacket)
{
	FAccount Account;
	Account.ID = TCHAR_TO_ANSI((FString)LoginPacket.UserName);
	Account.Password = TCHAR_TO_ANSI((FString)LoginPacket.Password);

	bool bFailed = false;
	if (Account.ID.size() > 15 || Account.ID.empty() ||
		Account.Password.size() > 15 || Account.Password.empty())
	{
		// TODO: 실패 패킷 전송
		return;
	}

	if (LoginPacket.bCreateAccount)
	{
		DBChannel->CreateAccount(Account,
			[Connection](bool bResult)
			{
				if (bResult)
				{
					FHaCreateAccountResult HaLoginResult;
					HaLoginResult.ResultCode = 1;
					FNetLoginMessage<NMT_STC_CreateAccountResult>::Send(Connection, HaLoginResult);
				}
				else
				{
					FHaCreateAccountResult HaLoginResult;
					HaLoginResult.ResultCode = 0;
					FNetLoginMessage<NMT_STC_CreateAccountResult>::Send(Connection, HaLoginResult);
				}
			}
		);
	}
	else
	{
		DBChannel->Login(Account,
			[this, Connection, Account](ELoginResult LoginResult)
			{
				FHaLoginResult HaLoginResult;
				HaLoginResult.ResultCode = LoginResult;
				FString UserName = ANSI_TO_TCHAR(Account.ID);
				FString Password = ANSI_TO_TCHAR(Account.Password);
				wmemcpy_s(HaLoginResult.UserName, ARRAYSIZE(HaLoginResult.UserName), UserName.data(), UserName.size());
				wmemcpy_s(HaLoginResult.Password, ARRAYSIZE(HaLoginResult.Password), Password.data(), Password.size());
				FNetLoginMessage<NMT_STC_LoginResult>::Send(Connection, HaLoginResult);
				if (LoginResult == ELoginResult::Success)
				{
					auto It = LoginUsers.find(Account.ID);
					if (It != LoginUsers.end())
					{
						if (It->second == Connection)
						{
							// 요청이 동일한 Connection에서 들어 온 경우
							// Send 버튼을 2번 이상 누른 상황일 수 있음
							return;
						}
						// 이미 로그인 되어 있는 경우
						E_LOG(Warning, TEXT("다른 곳에서 로그인 하여 기존 접속을 끊습니다: {}"), ANSI_TO_TCHAR(Account.ID));
						UIpConnection* OldConnection = It->second;
						LoginUsers.erase(Account.ID);
						OldConnection->Shutdown(); OldConnection = nullptr;

						for (UIpConnection* It : UEDediServers)
						{
							// 모든 데디 서버로 이 유저 로그아웃 처리를 하라고 요청
							FHa_DEDI_TO_LOGIN_SERVER_KickUser KickPacket;
							FString UserName = ANSI_TO_TCHAR(Account.ID);
							wmemcpy_s(KickPacket.UserName, ARRAYSIZE(KickPacket.UserName), UserName.data(), UserName.size());
							FNetDediServerChannelMessage<NMT_STUE_KickUser>::Send(It, KickPacket);
						}
					}

					OnLogin(Account, Connection);
				}
			}
		);
	}
}

void AServerGameMode::OnChatMessage(UIpConnection* Connection, FChatMessage& ChatMessage)
{
	FString Msg = ChatMessage.Message;
	E_LOG(Log, TEXT("[{}:{}]: {}"), Connection->GetRemoteIP(), Connection->GetRemotePort(), Msg);

	UIpNetDriver* NetDriver = dynamic_cast<UIpNetDriver*>(Connection->GetNetDriver());
	_ASSERT(NetDriver);

	{
		FString UserName = GetUsername(Connection);
		ZeroMemory(ChatMessage.UserName, sizeof(ChatMessage.UserName));
		std::wcsncpy(ChatMessage.UserName, UserName.c_str(), UserName.size());
	}
	map<UIpConnection*, TObjectPtr<UIpConnection>>& Connections = NetDriver->GetConnections();
	for (auto& It : Connections)
	{
		FNetChatMessage<NMT_STC_Chat>::Send((UNetConnection*)It.first, ChatMessage);
	}
}

void AServerGameMode::OnRequestDediServerInfo(UIpConnection* Connection)
{
	FHaDediServerInfo HaDediServerInfo;
	HaDediServerInfo.ServerCount = UEDediServers.size();
	for (int32 i = 0; i < HaDediServerInfo.ServerCount; ++i)
	{
		UIpConnection* DediServerConnection = UEDediServers[i];
		UUEDediServerChannel* DediServerChannel = dynamic_cast<UUEDediServerChannel*>(DediServerConnection->Channels[4].get());
		_ASSERT(DediServerChannel);

		string IP = TCHAR_TO_ANSI(DediServerChannel->Connection->GetRemoteIP());
		HaDediServerInfo.Ports[i] = DediServerChannel->Port; 
		HaDediServerInfo.CurrentPlayers[i] = DediServerChannel->CurrentPlayers;
		HaDediServerInfo.MaxPlayers[i] = DediServerChannel->MaxPlayers;
		memcpy_s(HaDediServerInfo.ServerIP[i], ARRAYSIZE(HaDediServerInfo.ServerIP[i]), IP.data(), IP.size());
	}
	FNetLoginMessage<NMT_STC_DediServerInfo>::Send(Connection, HaDediServerInfo);

}

void AServerGameMode::OnRequestCheckAccount(UIpConnection* Connection, FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValid& Bunch)
{
	FAccount Account;
	Account.ID = TCHAR_TO_ANSI(FString(Bunch.UserName));
	Account.Password = TCHAR_TO_ANSI(FString(Bunch.Password));
	DBChannel->Login(Account,
		[this, Connection, Account](ELoginResult LoginResult)
		{
			if (LoginResult != ELoginResult::Success)
			{
				E_LOG(Warning, TEXT("해커가 장난 치고 있는거 같아요"));
			}

			FHa_DEDI_TO_LOGIN_SERVER_CheckAccountValidResult ResultPacket;
			ResultPacket.bResult = LoginResult == ELoginResult::Success;
			FString UserName = ANSI_TO_TCHAR(Account.ID);
			FString Password = ANSI_TO_TCHAR(Account.Password);
			wmemcpy_s(ResultPacket.UserName, ARRAYSIZE(ResultPacket.UserName), UserName.data(), UserName.size());
			wmemcpy_s(ResultPacket.Password, ARRAYSIZE(ResultPacket.Password), Password.data(), Password.size());
			FNetDediServerChannelMessage<NMT_STUE_CheckAccountValid>::Send(Connection, ResultPacket);
		}
	);
}

FString AServerGameMode::GetUsername(UIpConnection* Connection)
{
	for (auto& It : LoginUsers)
	{
		if (It.second == Connection)
		{
			return ANSI_TO_TCHAR(It.first);
		}
	}
}
