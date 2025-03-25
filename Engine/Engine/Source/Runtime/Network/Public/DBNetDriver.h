#pragma once
#include "Engine/NetDriver.h"
#include "NetworkFwd.h"
#include <boost/mysql.hpp>
#include "DBNetDriver.generated.h"

class UDBConnection;
using FDBConnection = boost::mysql::pooled_connection;
using FStatement = boost::mysql::statement;

UCLASS()
class NETWORK_API UDBNetDriver : public UNetDriver
{
	GENERATED_BODY()

public:
	UDBNetDriver();
	~UDBNetDriver();
	virtual bool InitConnect(class FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error) override;

	FIOContext& GetContext() { return Context; }

	template<BOOST_MYSQL_EXECUTION_REQUEST ExecutionRequest, BOOST_MYSQL_RESULTS_TYPE ResultType> 
	bool SendSQL(shared_ptr<FDBConnection> InConnection, const ExecutionRequest& InExecutionRequest,
		ResultType& OutResult, boost::mysql::error_code& OutErrorCode, boost::mysql::diagnostics& OutDiagnostics)
	{
		InConnection->get().execute(InExecutionRequest, OutResult, OutErrorCode, OutDiagnostics);
		if (OutErrorCode)
		{
			PrintError(OutErrorCode, &OutDiagnostics);
			return false;
		}
		return true;
	}

	bool CreateStatement(shared_ptr<FDBConnection> Connection, string_view STMT, FStatement& OutStatement,
		boost::mysql::error_code& OutErrorCode, boost::mysql::diagnostics& OutDiagnostics);

	void GetConnection(function<void(shared_ptr<FDBConnection>)> Callback);

	TArray<TObjectPtr<UChannel>> Channels;

	virtual void Tick(float DeltaSeconds) override;

public:
	static void PrintError(boost::mysql::error_code& InErrorCode, boost::mysql::diagnostics* InDiagnostics = nullptr);

protected:
	FIOContext Context;

	unique_ptr<boost::mysql::connection_pool> DBConnectionPool;
};