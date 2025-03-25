#include "IpConnection.h"
#include "IpNetDriver.h"
#include "NetworkTypes.h"
#include "Engine/Channel.h"

bool UIpConnection::InitRemoteConnection(UNetDriver* InDriver, const FURL& InURL, FIOContext& InContext)
{
	bool bResult = InitBase(InDriver, InURL, InContext);
	if (bResult == false)
	{
		return bResult;
	}

	const bool bServer = InDriver->IsServer();
	if (!bServer)
	{
#if BOOST_VERSION >= 108700
		tcp::endpoint EndPoint(boost::asio::ip::make_address(TCHAR_TO_ANSI(URL.Host)), URL.Port);
#else
		tcp::endpoint EndPoint(boost::asio::ip::address::from_string(TCHAR_TO_ANSI(InURL.Host)), URL.Port);
#endif
		tcp::resolver Resolver(InContext);
		boost::system::error_code ErrorCode;
		boost::asio::connect(*GetSocket(), Resolver.resolve(EndPoint), ErrorCode);

		if (ErrorCode)
		{
			E_LOG(Error, TEXT("IpNetConnection Failed to connect: {}"), ANSI_TO_TCHAR(ErrorCode.message()));
			return false;
		}
	}

	return true;
}

void UIpConnection::OnAccepted()
{
	Super::OnAccepted();
	E_LOG(Log, TEXT("{} [Accepted]: {}:{}"), NetDriver->GetName(), GetRemoteIP(), GetRemotePort());
}

void UIpConnection::OnConnected()
{
	Super::OnConnected();
	E_LOG(Log, TEXT("{} [Connected]: {}:{}"), NetDriver->GetName(), GetRemoteIP(), GetRemotePort());
}

void UIpConnection::HandleConnectionEstablished()
{
	Super::HandleConnectionEstablished();

	SetConnectionState(EConnectionState::USOCK_Open);

	RecvBuffer.resize(RecvBufferSize);
	RemoteEndPoint = GetSocket()->remote_endpoint();
	RemoteIP = ANSI_TO_TCHAR(RemoteEndPoint.address().to_string());
	RemotePort = RemoteEndPoint.port();

	BufferPool = make_unique<boost::pool<>>(RecvBufferSize);
}

void UIpConnection::ReceivedRawPacket(uint64 InRecvSize)
{
	FBunch* Bunch = reinterpret_cast<FBunch*>(RecvBuffer.data());

	const int32 ChannelIndex = Bunch->ChannelIndex;
	if (Channels.size() <= ChannelIndex || !Channels[ChannelIndex])
	{
		E_LOG(Error, TEXT("{} Wrong Channel Index:{} [{}:{}]"), NetDriver->GetName(), ChannelIndex, RemoteIP, RemotePort);
		// TODO: Shutdown
		return;
	}
	Channels[ChannelIndex]->ReceivedBunch(*Bunch);

	//E_LOG(Log, TEXT("Bunch Test: {} PSize: {}"), Bunch->Test, Bunch->PacketSize);
}

void UIpConnection::LowLevelSend(void* Data, int32 CountBits)
{
	if (Data == nullptr || CountBits == 0)
	{
		E_LOG(Error, TEXT("{} LowLevelSend Wrong : CountBits: {}"), NetDriver->GetName(), CountBits);
		return;
	}

	void* Buffer = BufferPool->malloc();
	memcpy(Buffer, Data, CountBits);

	boost::asio::async_write(*GetSocket(), boost::asio::buffer(Buffer, CountBits),
		[this, Buffer](const boost::system::error_code& ErrorCode, uint64)
		{
			BufferPool->free(Buffer);
			if (ErrorCode)
			{
				E_LOG(Error, TEXT("{} [Send failed] {} {}:{}"), NetDriver->GetName(), ANSI_TO_TCHAR(ErrorCode.message()), RemoteIP, RemotePort);
				// Shutdown
				return;
			}
		});
}

bool UIpConnection::InitBase(UNetDriver* InDriver, const FURL& InURL, FIOContext& InContext)
{
	bool bResult = Super::InitBase(InDriver, InURL);

	Socket = make_unique<FSocket>(InContext);

	return bResult;
}

void UIpConnection::Shutdown()
{
	Super::Shutdown();

	if (ConnectionState == EConnectionState::USOCK_Closed) { return; }

	SetConnectionState(EConnectionState::USOCK_Closed);
	Socket->close();
	NetDriver->CloseConnection(this);
}