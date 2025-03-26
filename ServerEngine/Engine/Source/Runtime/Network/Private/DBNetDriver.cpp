#include "DBNetDriver.h"
#include "NetworkTypes.h"
#include <boost/asio/ip/tcp.hpp>

UDBNetDriver::UDBNetDriver()
{
	// boost mysql connection을 사용 할 것이므로 이 Connection을 사용하지 않음
	SetNetConnectionClass(UNetConnection::StaticClass());
}

UDBNetDriver::~UDBNetDriver()
{
	if (HasAnyFlags(EObjectFlags::RF_ClassDefaultObject)) { return; }
	if (!DBConnectionPool) { return; }

	DBConnectionPool->cancel();
}

//UIpConnection* UIpNetDriver::GetServerConnection()
//{
//	if (IsServer())
//	{
//		_ASSERT(false);
//		return nullptr;
//	}
//
//	return ServerConnection.get();
//}

bool UDBNetDriver::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	if (!Super::InitConnect(InNotify, ConnectURL, Error))
	{
		return false;
	}

	string ID = TCHAR_TO_ANSI(URL.Op[0]);
	string Password = TCHAR_TO_ANSI(URL.Op[1]);

	boost::mysql::pool_params Params;
	Params.server_address.emplace_host_and_port(TCHAR_TO_ANSI(URL.Host), URL.Port);
	Params.username = ID;
	Params.password = Password;
#if WITH_DEBUG
	Params.connect_timeout = std::chrono::seconds(60); // 디버깅 하다가 타임아웃 되는 상황 방지
#endif
	Params.multi_queries = false;
#if BOOST_VERSION >= 108700
	DBConnectionPool = make_unique<boost::mysql::connection_pool>(Context, move(Params));
#else
	DBConnectionPool = make_unique<boost::mysql::connection_pool>(
		boost::mysql::pool_executor_params::thread_safe(Context.get_executor()),
		move(Params));
#endif

	DBConnectionPool->async_run(boost::asio::detached);

	// 적당히 2개정도 확보 해둠
	// 필요에 따라 수정해서 쓰세요
	Channels.resize(2);

	return true;
}


void UDBNetDriver::PrintError(boost::mysql::error_code& InErrorCode, boost::mysql::diagnostics* InDiagnostics)
{
	if (InErrorCode)
	{
		E_LOG(Warning, TEXT("{}"), ANSI_TO_TCHAR(InErrorCode.to_string()));
	}
	if (InDiagnostics)
	{
		if (!InDiagnostics->server_message().empty())
		{
			E_LOG(Warning, TEXT("ServerMessage: {}"), ANSI_TO_TCHAR(InDiagnostics->server_message().data()));
		}
		else if (!InDiagnostics->client_message().empty())
		{
			E_LOG(Warning, TEXT("ClientMessage: {}"), ANSI_TO_TCHAR(InDiagnostics->client_message().data()));
		}
	}
}

bool UDBNetDriver::CreateStatement(shared_ptr<FDBConnection> Connection, string_view STMT, FStatement& OutStatement
	, boost::mysql::error_code& OutErrorCode, boost::mysql::diagnostics& OutDiagnostics)
{
	OutStatement = Connection->get().prepare_statement(STMT, OutErrorCode, OutDiagnostics);
	if (OutErrorCode)
	{
		UDBNetDriver::PrintError(OutErrorCode, &OutDiagnostics);
		return false;
	}
	return true;
}

void UDBNetDriver::GetConnection(function<void(shared_ptr<FDBConnection>)> Callback)
{
	shared_ptr<boost::mysql::diagnostics> Diagnostics = make_shared<boost::mysql::diagnostics>();
	DBConnectionPool->async_get_connection(*Diagnostics, [Callback, Diagnostics](boost::mysql::error_code ErrorCode, FDBConnection Connection)
		{
			if (ErrorCode)
			{
				UDBNetDriver::PrintError(ErrorCode, Diagnostics.get());
				return;
			}

			shared_ptr<FDBConnection> SharedConnection = make_shared<FDBConnection>(move(Connection));
			Callback(SharedConnection);
		});
}

void UDBNetDriver::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	uint32 PollCount = 0;
	const uint32 MaxPollCountPerTick = 1000;

#if BOOST_VERSION >= 108700
	try
	{
		while (uint32 n = Context.poll_one())
		{
			PollCount += n;
			if (PollCount >= MaxPollCountPerTick)
			{
				break;
			}
		}
	}
	catch (boost::system::error_code ErrorCode)
	{
		if (ErrorCode)
		{
			E_LOG(Error, TEXT("Context poll error: {}"), ANSI_TO_TCHAR(ErrorCode.message()));
		}
	}
#else
	boost::system::error_code ErrorCode;
	while (uint32 n = Context.poll_one(ErrorCode))
	{
		if (ErrorCode)
		{
			E_LOG(Error, TEXT("Context poll error: {}"), ANSI_TO_TCHAR(ErrorCode.message()));
			break;
		}
		PollCount += n;
		if (PollCount >= MaxPollCountPerTick)
		{
			break;
		}
	}
#endif
}