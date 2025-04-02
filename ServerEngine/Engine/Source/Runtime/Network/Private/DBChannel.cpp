#include "Engine/NetConnection.h"
#include "DBChannel.h"
#include "DBNetDriver.h"
#include <boost/mysql.hpp>

void UDBChannel::Init(UNetConnection* InConnection, int32 InChIndex)
{
	Super::Init(InConnection, InChIndex);

	DBNetDriver = dynamic_cast<UDBNetDriver*>(NetDriver);
	boost::asio::ssl::context SSLContext(boost::asio::ssl::context::tls_client);

	FURL& URL = DBNetDriver->URL;

	string ID = TCHAR_TO_ANSI(URL.Op[0]);
	string Password = TCHAR_TO_ANSI(URL.Op[1]);

	boost::mysql::handshake_params HandshakeParams(ID, Password);
	boost::mysql::tcp_ssl_connection Connection(DBNetDriver->GetContext(), SSLContext);

	boost::mysql::error_code ErrorCode;
	boost::mysql::diagnostics Diagnostics;

#if BOOST_VERSION >= 108700
	tcp::endpoint EndPoint(boost::asio::ip::make_address(TCHAR_TO_ANSI(URL.Host)), URL.Port);
#else
	tcp::endpoint EndPoint(boost::asio::ip::address::from_string(TCHAR_TO_ANSI(URL.Host)), URL.Port);
#endif
	Connection.connect(EndPoint, HandshakeParams, ErrorCode, Diagnostics);
	if (ErrorCode)
	{
		E_LOG(Error, TEXT("{} IP:{}:{} ID:{}, PW:{} connect failed!"), GetName(), URL.Host, to_wstring(URL.Port), ANSI_TO_TCHAR(ID), ANSI_TO_TCHAR(Password));
		UDBNetDriver::PrintError(ErrorCode);

		return;
	}

	E_LOG(Log, TEXT("{} IP:{}:{} ID:{}, PW:{} connected!"), GetName(), URL.Host, to_wstring(URL.Port), ANSI_TO_TCHAR(ID), ANSI_TO_TCHAR(Password));

	boost::mysql::results Result;
	Connection.execute("CREATE DATABASE IF NOT EXISTS UserDB", Result, ErrorCode, Diagnostics);
	Connection.execute("USE UserDB", Result, ErrorCode, Diagnostics);
	Connection.execute(R"(
					CREATE TABLE IF NOT EXISTS Account
					(
						UserName VARCHAR(15) PRIMARY KEY,
						Password VARCHAR(15) NOT NULL
					);
				)", Result, ErrorCode, Diagnostics);
	Connection.close();

	// 추가
	//{
	//	for (int i = 0; i < 100; ++i)
	//	{
	//		string ID = "Hello" + to_string(i);
	//		string Password = "password";
	//		string SQL = std::format("INSERT INTO Account VALUES('{}', '{}')", ID, Password);
	//		Connection.execute(SQL, Result, ErrorCode, Diagnostics);
	//		if (ErrorCode)
	//		{
	//			PrintDBError(ErrorCode, &Diagnostics);
	//		}
	//	}
	//}
	//// 삭제
	//{
	//	// 대상 조회
	//	{
	//		boost::mysql::static_results<FAccount> Accounts;
	//		Connection.execute("SELECT UserName, Password FROM Account WHERE UserName='Hello0'", Accounts, ErrorCode, Diagnostics);
	//		size_t FoundSize = Accounts.rows().size();
	//		if (FoundSize == 1)
	//		{
	//			E_LOG(Log, TEXT("Found!"));
	//		}
	//		else
	//		{
	//			E_LOG(Log, TEXT("Not Found!"));
	//		}
	//	}
	//	Connection.execute("DELETE FROM Account WHERE UserName='Hello0'", Result, ErrorCode, Diagnostics);
	//	if (ErrorCode)
	//	{
	//		PrintDBError(ErrorCode, &Diagnostics);
	//	}
	//	{
	//		boost::mysql::static_results<FAccount> Accounts;
	//		Connection.execute("SELECT UserName, Password FROM Account WHERE UserName='Hello0'", Accounts, ErrorCode, Diagnostics);
	//		size_t FoundSize = Accounts.rows().size();
	//		if (FoundSize == 1)
	//		{
	//			E_LOG(Log, TEXT("Found!"));
	//		}
	//		else
	//		{
	//			E_LOG(Log, TEXT("Not Found!"));
	//		}
	//	}
	//}
	//// 조회
	//{
	//	{
	//		Connection.execute("SELECT UserName, Password FROM Account", Result, ErrorCode, Diagnostics);
	//		for (const auto& Row : Result.rows())
	//		{
	//			FAccount Account;
	//			Account.ID = Row[0].as_string();
	//			Account.Password = Row[1].as_string();

	//			E_LOG(Log, TEXT("UserName: {}, Password: {}"), ANSI_TO_TCHAR(Account.ID), ANSI_TO_TCHAR(Account.Password));
	//		}
	//	}
	//	{
	//		boost::mysql::static_results<FAccount> Accounts;
	//		Connection.execute("SELECT UserName, Password FROM Account", Accounts, ErrorCode, Diagnostics);
	//		for(const FAccount& Account : Accounts.rows())
	//		{
	//			E_LOG(Log, TEXT("UserName: {}, Password: {}"), ANSI_TO_TCHAR(Account.ID), ANSI_TO_TCHAR(Account.Password));
	//		}
	//	}
	//}


}

void UDBChannel::CreateAccount(const FAccount& InAccount, function<void(bool)> ResultFunction)
{
	DBNetDriver->GetConnection([this, InAccount, ResultFunction](shared_ptr<FDBConnection> NewConnection)
		{
			AsyncTask<ENamedThreads::AnyThread>(
				[this, NewConnection, InAccount, ResultFunction]
				{
					SetThreadDescription(GetCurrentThread(), TEXT("DB Login Thread"));

					boost::mysql::error_code ErrorCode;
					boost::mysql::diagnostics Diagnostics;
					boost::mysql::results Result;
					DBNetDriver->SendSQL(NewConnection, "USE UserDB", Result, ErrorCode, Diagnostics);

					string STMT = "INSERT INTO Account VALUES(?, ?)";
					FStatement Statement;
					// SQL Inejction 방어 코드가 들어가 있는 Statement 쿼리를 사용 하면 안전하게 파라미터를 전달 할 수 있다
					if (!DBNetDriver->CreateStatement(NewConnection, STMT, Statement, ErrorCode, Diagnostics))
					{
						return;
					}
					TArray<boost::mysql::field> Fields; Fields.reserve(2);
					Fields.emplace_back(InAccount.UserName);
					Fields.emplace_back(InAccount.Password);
					bool bResult = DBNetDriver->SendSQL(NewConnection, Statement.bind(Fields.begin(), Fields.end()), Result, ErrorCode, Diagnostics);

					if (bResult)
					{
						E_LOG(Log, TEXT("Create account: {}, {}"), ANSI_TO_TCHAR(InAccount.UserName), ANSI_TO_TCHAR(InAccount.Password));
					}
					else
					{
						E_LOG(Warning, TEXT("Create account Failed: {}, {}"), ANSI_TO_TCHAR(InAccount.UserName), ANSI_TO_TCHAR(InAccount.Password));
					}

					AsyncTask<ENamedThreads::GameThread>(
						[this, bResult, ResultFunction]
						{
							ResultFunction(bResult);
						}
					);

					SetThreadDescription(GetCurrentThread(), TEXT("Boost Task Thread"));
				}
			);
		}
	);
}

void UDBChannel::Login(const FAccount& InAccount, function<void(ELoginResult)> ResultFunction)
{
	DBNetDriver->GetConnection([this, InAccount, ResultFunction](shared_ptr<FDBConnection> NewConnection)
		{
			AsyncTask<ENamedThreads::AnyThread>(
				[this, NewConnection, InAccount, ResultFunction]
				{
					SetThreadDescription(GetCurrentThread(), TEXT("DB Login Thread"));

					boost::mysql::error_code ErrorCode;
					boost::mysql::diagnostics Diagnostics;
					boost::mysql::results Result;
					bool bResult =DBNetDriver->SendSQL(NewConnection, "USE UserDB", Result, ErrorCode, Diagnostics);

					string STMT = "SELECT UserName, Password FROM Account WHERE UserName=?";
					FStatement Statement;
					// SQL Inejction 방어 코드가 들어가 있는 Statement 쿼리를 사용 하면 안전하게 파라미터를 전달 할 수 있다
					if (!DBNetDriver->CreateStatement(NewConnection, STMT, Statement, ErrorCode, Diagnostics))
					{
						return;
					}
					TArray<boost::mysql::field> Fields; Fields.reserve(2);
					Fields.emplace_back(InAccount.UserName);
					boost::mysql::static_results<FAccount> Accounts;
					bResult = DBNetDriver->SendSQL(NewConnection, Statement.bind(Fields.begin(), Fields.end()), Accounts, ErrorCode, Diagnostics);
					_ASSERT(bResult);
					size_t Size = Accounts.rows().size();
					ELoginResult LoginResult = ELoginResult::None; 

					if (Size == 0)
					{
						// 해당 계정을 찾을 수 없는 경우
						LoginResult = ELoginResult::UsernameNotFound;
						E_LOG(Warning, TEXT("Login failed [UsernameNotFound]: {}, {}"), ANSI_TO_TCHAR(InAccount.UserName), ANSI_TO_TCHAR(InAccount.Password));
					}
					else if (Size == 1)
					{
						// 해당 계정을 찾음
						FAccount Account = Accounts.rows().front();
						bool bSame = Account.Password == InAccount.Password;
						if (bSame)
						{
							// 로그인 성공
							LoginResult = ELoginResult::Success;
							E_LOG(Log, TEXT("Login: {}, {}"), ANSI_TO_TCHAR(InAccount.UserName), ANSI_TO_TCHAR(InAccount.Password));
						}
						else
						{
							// 비밀번호가 틀림
							LoginResult = ELoginResult::PasswordError;
							E_LOG(Warning, TEXT("Login failed [PasswordError]: {}, {}"), ANSI_TO_TCHAR(InAccount.UserName), ANSI_TO_TCHAR(InAccount.Password));
						}
					}

					AsyncTask<ENamedThreads::GameThread>(
						[this, LoginResult, ResultFunction]
						{
							ResultFunction(LoginResult);
						}
					);

					SetThreadDescription(GetCurrentThread(), TEXT("Boost Task Thread"));
				}
			);
		}
	);
}
