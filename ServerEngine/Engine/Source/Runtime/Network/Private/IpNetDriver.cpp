#include "IpNetDriver.h"
#include "NetworkTypes.h"
#include "IpConnection.h"
#include <boost/asio/ip/tcp.hpp>
#include "Net/DataBunch.h"
#include "NetworkDelegates.h"

UIpNetDriver::UIpNetDriver()
{
	SetNetConnectionClass(UIpConnection::StaticClass());
}

UIpConnection* UIpNetDriver::GetServerConnection()
{
	if (IsServer())
	{
		_ASSERT(false);
		return nullptr;
	}

	return ServerConnection.get();
}

void UIpNetDriver::CloseConnection(UNetConnection* Connection)
{
	Connection->Shutdown();
	UIpConnection* IpConnection = dynamic_cast<UIpConnection*>(Connection);
	if (Backlogs.contains(IpConnection))
	{
		Backlogs.erase(IpConnection);
	}
	if (Connections.contains(IpConnection))
	{
		Notify->NotifyConnectionClosed(IpConnection);
		Connections.erase(IpConnection);
	}
}

bool UIpNetDriver::InitBase(bool bInitAsClient, FNetworkNotify* InNotify, const FURL& LocalURL, bool bReuseAddressAndPort, FString& Error)
{
	if (!Super::InitBase(false, InNotify, LocalURL, bReuseAddressAndPort, Error))
	{
		E_LOG(Warning, TEXT("Failed to init net driver ListenURL: {}: {}"), LocalURL.ToString(), Error);
		return false;
	}

	return true;
}

bool UIpNetDriver::InitListen(FNetworkNotify* InNotify, FURL& ListenURL, bool bReuseAddressAndPort, FString& Error)
{
	if (!Super::InitListen(InNotify, ListenURL, bReuseAddressAndPort, Error))
	{
		return false;
	}

	E_LOG(Log, TEXT("{} Server Listen"), GetName());

	Acceptor = make_shared<FAcceptor>(Context,
		tcp::endpoint(tcp::v4(), (port_type)ListenURL.Port), bReuseAddressAndPort);

	const int32 AsyncAceeptCount = 10;
	for (int32 i = 0; i < AsyncAceeptCount; ++i)
	{
		StartAceept();
	}

	return true;
}

bool UIpNetDriver::InitConnect(FNetworkNotify* InNotify, const FURL& ConnectURL, FString& Error)
{
	if (!Super::InitConnect(InNotify, ConnectURL, Error))
	{
		return false;
	}

	ServerConnection = NewObject<UIpConnection>(this, NetConnectionClass);
	bool bResult = ServerConnection->InitRemoteConnection(this, URL, Context);
	if (bResult)
	{
		ServerConnection->OnConnected();

		AsyncRead(ServerConnection.get());
	}
	return bResult;
}

void UIpNetDriver::Tick(float DeltaSeconds)
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

void UIpNetDriver::StartAceept()
{
	TObjectPtr<UIpConnection> NetConnection = NewObject<UIpConnection>(this, NetConnectionClass);
	NetConnection->InitRemoteConnection(this, URL, Context);
	Backlogs.emplace(NetConnection.get(), NetConnection);

	Acceptor->async_accept(*NetConnection->GetSocket(),
		[this, NetConnection](const boost::system::error_code& Error)
		{
			UIpConnection* IpConnection = NetConnection.get();
			Backlogs.erase(IpConnection);
			StartAceept();

			if (Error)
			{
				E_LOG(Error, TEXT("{} Accept failed: {}"), GetName(), ANSI_TO_TCHAR(Error.message()));
				return;
			}
			IpConnection->OnAccepted();
			Connections.emplace(IpConnection, NetConnection);
			Notify->NotifyAcceptedConnection(IpConnection);

			AsyncRead(IpConnection);
		}
	);
}

void UIpNetDriver::AsyncRead(UIpConnection* IpConnection)
{
	boost::asio::async_read(*IpConnection->GetSocket(),
		boost::asio::buffer(IpConnection->GetRecvBuffer().data(), sizeof(FPacketHeader)),
		[this, IpConnection](const boost::system::error_code& InError, uint64 InRecvSize)
		{
			if (InError)
			{
				if (Connections.contains(IpConnection))
				{
					Notify->NotifyConnectionClosed(IpConnection);
					Connections.erase(IpConnection);
				}
				E_LOG(Log, TEXT("{} [Read Failed]: {}, {}:{}"), GetName(), ANSI_TO_TCHAR(InError.message()), IpConnection->GetRemoteIP(), IpConnection->GetRemotePort());
				return;
			}

			FPacketHeader* PacketHeader = reinterpret_cast<FPacketHeader*>(IpConnection->GetRecvBuffer().data());
			if (PacketHeader->PacketSize < sizeof(FPacketHeader))
			{
				E_LOG(Error, TEXT("{} PacketSize({}) < sizeof(FPacketHeader): {}:{}"), GetName(), PacketHeader->PacketSize, IpConnection->GetRemoteIP(), IpConnection->GetRemotePort());
				// 우리 정책을 따르지 않은 이상한 Packet
				// TODO Shutdown()
				return;
			}

			// PacketSize가 우리가 정한 최대 사이즈보다 크다
			if (IpConnection->GetRecvBufferSize() < PacketHeader->PacketSize)
			{
				E_LOG(Warning, TEXT("{} IpConnection->GetRecvBufferSize({}) < PacketHeader->PacketSize: {}:{}"), GetName(), IpConnection->GetRecvBufferSize(), IpConnection->GetRemoteIP(), IpConnection->GetRemotePort());
				// TODO Shutdown()
				return;
			}

			int32 RemainPacketSize = PacketHeader->PacketSize - sizeof(FPacketHeader);
			boost::asio::async_read(*IpConnection->GetSocket(),
				boost::asio::buffer(IpConnection->GetRecvBuffer().data() + sizeof(FPacketHeader), RemainPacketSize),
				[this, IpConnection](const boost::system::error_code& Error, uint64 InRecvSize)
				{
					if (Error)
					{
						if (Connections.contains(IpConnection))
						{
							Notify->NotifyConnectionClosed(IpConnection);
							Connections.erase(IpConnection);
						}
						E_LOG(Log, TEXT("{} [Read Failed]: {}, {}:{}"), GetName(), ANSI_TO_TCHAR(Error.message()), IpConnection->GetRemoteIP(), IpConnection->GetRemotePort());
						return;
					}
					//IpConnection->ReceivedRawPacket(InRecvSize + sizeof(FPacketHeader));
					IpConnection->ReceivedRawPacket(InRecvSize + sizeof(FPacketHeader));

					AsyncRead(IpConnection);
				}
			);
		}
	);
}
